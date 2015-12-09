#include <iostream>

#include <string>
#include <new>
#include <semaphore.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#include <shm/SharedMemoryHelper.hxx>
#include "QueueItem.hxx"
#include "RawQueue.hxx"
#include "IPCQueue.hxx"

bool IPCQueue::Wait()
{
	return sem_wait(queue_operation_semaphore) == 0;
}

bool IPCQueue::Post()
{
	return sem_post(queue_operation_semaphore) == 0;
}

bool IPCQueue::TryWait()
{
	return sem_trywait(queue_operation_semaphore) == 0;
}

IPCQueue::IPCQueue(const std::string& queue_name)
	:	queue_shared_memory(NULL),
		queue_operation_semaphore(SEM_FAILED),
		memory_prepare_semaphore(SEM_FAILED),
		elem_count_semaphore(SEM_FAILED),
		shm_fd(-1),
		queue_name(queue_name),
		deletion_fd_protection(-1)
{
	deletion_fd_protection = open(("/tmp/deletion_fd_protection.ipc_lockcheck." + queue_name).c_str(), O_CREAT | O_RDWR);
	if (deletion_fd_protection == -1)
	{
		throw std::exception(/*"Cannot access critical lock file"*/);
	}

	if (flock(deletion_fd_protection, LOCK_EX | LOCK_NB) == 0)
	{
		//we have the only and only lock now so this below should generrally NOT be able to fail
		flock(deletion_fd_protection, LOCK_UN);
		if (flock(deletion_fd_protection, LOCK_SH | LOCK_NB) != 0)
		{
			throw std::exception(/*"Cannot access critical lock file"*/);
		}

		shm_unlink(queue_name.c_str());
		sem_unlink(queue_name.c_str());
		sem_unlink(("SHM_PROT_" + queue_name).c_str());
		sem_unlink(("ITEM_COUNTER_" + queue_name).c_str());
	}
	else
	{
		if (flock(deletion_fd_protection, LOCK_SH | LOCK_NB) != 0)
		{
			throw std::exception(/*"Cannot access critical lock file"*/);
		}
	}

	my_PrepSem("ITEM_COUNTER_" + queue_name, &elem_count_semaphore, 0);
	my_PrepSem("SHM_PROT_" + queue_name, &memory_prepare_semaphore, 1);
	my_PrepSem(queue_name, &queue_operation_semaphore, 1);

	sem_wait(memory_prepare_semaphore);

	queue_shared_memory = my_shm_create<RawQueue*>(queue_name, RawQueue::GetSizeBytes(), shm_fd);
	if (queue_shared_memory)
	{
		// use C++98 placement new to call constructor when new shared memory region has been created
		new (queue_shared_memory) RawQueue();
	}
	else
	{
		queue_shared_memory = my_shm_open<RawQueue*>(queue_name, shm_fd);
		if (!queue_shared_memory)
		{
			throw std::exception(/*"Cannot access shared memory"*/);
		}
	}

	sem_post(memory_prepare_semaphore);
}

bool IPCQueue::Push(QueueItem* item)
{
	Wait();
	bool ret_val = queue_shared_memory->Push(item);
	Post();
	sem_post(elem_count_semaphore);
	return ret_val;
}

bool IPCQueue::Pop(QueueItem* item)
{
	sem_wait(elem_count_semaphore);
	Wait();
	bool ret_val = queue_shared_memory->Pop(item);
	Post();
	return ret_val;
}

bool IPCQueue::TryPush(QueueItem* item, bool only_try)
{
	if (!TryWait())
	{
		return false;
	}
	bool ret_val = queue_shared_memory->Push(item);
	Post();
	sem_post(elem_count_semaphore);
	return ret_val;
}

bool IPCQueue::TryPop(QueueItem* item)
{
	if (sem_trywait(queue_operation_semaphore) != 0)
	{
		return false;
	}
	sem_wait(elem_count_semaphore);
	if (!TryWait())
	{
		sem_post(elem_count_semaphore);
		return false;
	}
	bool ret_val = queue_shared_memory->Pop(item);
	Post();
	return ret_val;
}

size_t IPCQueue::Count() const
{
	int count = 0;
	sem_getvalue(elem_count_semaphore, &count);
	return (size_t)count;
	//return queue_shared_memory->Count();
}

IPCQueue::~IPCQueue()
{
	sem_close(queue_operation_semaphore);
	sem_close(memory_prepare_semaphore);
	close(shm_fd);

	if (deletion_fd_protection != -1)
	{
		flock(deletion_fd_protection, LOCK_UN); // release shared lock
		close(deletion_fd_protection);
	}
}

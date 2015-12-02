#ifndef IPCQUEUE_H
#define IPCQUEUE_H

#include <iostream>

#include <string>
#include <new>
#include <semaphore.h>
#include <pthread.h>

#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

#include "SharedMemoryHelper.hxx"
#include "QueueItem.hxx"
#include "RawQueue.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class IPCQueue
{
public:
	typedef RawQueue<max_data_size_bytes, max_items> _RawQueue;
	typedef typename _RawQueue::_Item _Item;
private:
	_RawQueue* queue_shared_memory;
	sem_t* queue_operation_semaphore;
	sem_t* memory_prepare_semaphore;
	int shm_fd;
	std::string queue_name;
	int deletion_fd_protection;

	/* Why geen mutex? [maar wel memory_prepare_semaphore] :
		For inter-process synchronization, a mutex needs to be allo-
		cated   in  memory shared between these processes. Since the
		memory for such a mutex must be allocated dynamically,   the
		mutex needs to be explicitly initialized using mutex_init().

		We zijn shared memory aan t maken en zorgen ervoor dat er maar 1 creator is, dus nutteloos eh? Jup!
	*/

	void PrepSem(const std::string& name, sem_t** semaphore, int initial)
	{
		*semaphore = sem_open(name.c_str(), O_CREAT | O_EXCL, 0777, initial);

		if (*semaphore == SEM_FAILED)
		{
			*semaphore = sem_open(name.c_str(), 0);
			if (*semaphore == SEM_FAILED)
			{
				throw std::exception(/*"Cannot access semaphore"*/);
			}
		}
	}
public:
	IPCQueue(const std::string& queue_name)
		:	queue_shared_memory(NULL),
			queue_operation_semaphore(SEM_FAILED),
			memory_prepare_semaphore(SEM_FAILED),
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
			sem_unlink((queue_name).c_str());
			sem_unlink(("SHM_PROT_" + queue_name).c_str());
		}
		else
		{
			if (flock(deletion_fd_protection, LOCK_SH | LOCK_NB) != 0)
			{
				throw std::exception(/*"Cannot access critical lock file"*/);
			}
		}

		PrepSem("SHM_PROT_" + queue_name, &memory_prepare_semaphore, 1);
		PrepSem(queue_name, &queue_operation_semaphore, 1);

		sem_wait(memory_prepare_semaphore);

		queue_shared_memory = my_shm_create<_RawQueue*>(queue_name, _RawQueue::GetSizeBytes(), shm_fd);
		if (queue_shared_memory)
		{
			// use C++98 placement new to call constructor when new shared memory region has been created
			new (queue_shared_memory) _RawQueue();
		}
		else
		{
			queue_shared_memory = my_shm_open<_RawQueue*>(queue_name, shm_fd);
			if (!queue_shared_memory)
			{
				throw std::exception(/*"Cannot access shared memory"*/);
			}
		}

		sem_post(memory_prepare_semaphore);
	}

	bool Wait()
	{
		return sem_wait(queue_operation_semaphore) == 0;
	}

	bool Post()
	{
		return sem_post(queue_operation_semaphore) == 0;
	}

	bool TryWait()
	{
		return sem_trywait(queue_operation_semaphore) == 0;
	}

	bool Push(_Item* item)
	{
		return queue_shared_memory->Push(item);
	}

	bool Pop(_Item* item)
	{
		return queue_shared_memory->Pop(item);
	}

	size_t Count()
	{
		return queue_shared_memory->Count();
	}

	~IPCQueue()
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
};

#endif
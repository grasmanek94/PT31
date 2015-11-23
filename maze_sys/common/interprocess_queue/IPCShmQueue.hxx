#ifndef IPCQUEUE_H
#define IPCQUEUE_H

#include <string>
#include <new>
#include <semaphore.h>

#include "SharedMemoryHelper.hxx"
#include "QueueItem.hxx"
#include "RawShmQueue.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class IPCShmQueue
{
public:
	typedef RawShmQueue<max_data_size_bytes, max_items> _RawQueue;
	typedef typename _RawQueue::_Item _Item;
private:
	_RawQueue* queue_shared_memory;
	sem_t* queue_operation_semaphore;
	sem_t* memory_prepare_semaphore;
	sem_t* queue_counter_semaphore;
	int shm_fd;
	std::string queue_name;

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

	_RawQueue* Queue()
	{
		return queue_shared_memory;
	}
public:
	IPCShmQueue(const std::string& queue_name)
		:	queue_shared_memory(NULL),
			queue_operation_semaphore(SEM_FAILED),
			memory_prepare_semaphore(SEM_FAILED),
			queue_counter_semaphore(SEM_FAILED),
			shm_fd(-1),
			queue_name(queue_name)
	{
		PrepSem("QUEUE_CTR_" + queue_name, &queue_counter_semaphore, 0);
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
		return Queue()->Push(item, queue_counter_semaphore);
	}

	bool Pop(_Item* item)
	{
		return Queue()->Pop(item, queue_counter_semaphore);
	}

	size_t Count()
	{
		return Queue()->Count(queue_counter_semaphore);
	}

	~IPCShmQueue()
	{
		/*close(shm_fd);
		shm_unlink(queue_name.c_str());

		sem_close(queue_operation_semaphore);
		sem_unlink(queue_name.c_str());

		sem_close(memory_prepare_semaphore);		
		sem_unlink(("SHM_PROT_" + queue_name).c_str());

		sem_close(queue_counter_semaphore);
		sem_unlink(("QUEUE_CTR_" + queue_name).c_str());*/
	}
};

#endif
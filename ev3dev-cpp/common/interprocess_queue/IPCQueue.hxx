#ifndef IPCQUEUE_H
#define IPCQUEUE_H

#include <iostream>
#include <string>

#include <string>
#include <new>
#include <semaphore.h>

#include "SharedMemoryHelper.hxx"
#include "QueueItem.hxx"
#include "RawQueue.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class IPCQueue
{
public:
	typedef RawQueue<max_data_size_bytes, max_items> _RawQueue;
private:
	_RawQueue* queue_shared_memory;
	sem_t* queue_operation_semaphore;
	int shm_fd;
	std::string queue_name;
public:
	IPCQueue(const std::string& queue_name)
		: queue_shared_memory(NULL),
		queue_operation_semaphore(SEM_FAILED),
		shm_fd(0),
		queue_name(queue_name)
	{
		std::cout << "TRYING: queue_operation_semaphore = sem_open(queue_name.c_str(), O_CREAT | O_EXCL, 0777, 1)" << std::endl;
		queue_operation_semaphore = sem_open(queue_name.c_str(), O_CREAT | O_EXCL, 0777, 1);

		if (queue_operation_semaphore == SEM_FAILED)
		{
			std::cout << "FAILED: queue_operation_semaphore = sem_open(queue_name.c_str(), O_CREAT | O_EXCL, 0777, 1)" << std::endl;
			std::cout << "TRYING: queue_operation_semaphore = sem_open(queue_name.c_str(), 0)" << std::endl;
			queue_operation_semaphore = sem_open(queue_name.c_str(), 0);
			if (queue_operation_semaphore == SEM_FAILED)
			{
				std::cout << "FAILED: queue_operation_semaphore = sem_open(queue_name.c_str(), 0)" << std::endl;
				throw std::exception(/*"Cannot access semaphore"*/);
			}
			else
			{
				std::cout << "SUCCESS: queue_operation_semaphore = sem_open(queue_name.c_str(), 0)" << std::endl;
			}
		}
		else
		{
			std::cout << "SUCCESS: queue_operation_semaphore = sem_open(queue_name.c_str(), O_CREAT | O_EXCL, 0777, 1)" << std::endl;
		}

		std::cout << "DOING: EndOperation()" << std::endl;
		EndOperation();
		std::cout << "DOING: BeginOperation()" << std::endl;
		BeginOperation();

		std::cout << "DOING: queue_shared_memory = my_shm_create<_RawQueue*>(queue_name, _RawQueue::GetSizeBytes(), shm_fd)" << std::endl;
		queue_shared_memory = my_shm_create<_RawQueue*>(queue_name, _RawQueue::GetSizeBytes(), shm_fd);
		if (queue_shared_memory)
		{
			std::cout << "DOING: new (queue_shared_memory) _RawQueue()" << std::endl;
			// use C++98 placement new to call constructor when new shared memory region has been created
			new (queue_shared_memory) _RawQueue();
		}
		else
		{
			std::cout << "TRYING: queue_shared_memory = my_shm_open<_RawQueue*>(queue_name, shm_fd)" << std::endl;
			queue_shared_memory = my_shm_open<_RawQueue*>(queue_name, shm_fd);
			if (!queue_shared_memory)
			{
				std::cout << "FAILED: queue_shared_memory = my_shm_open<_RawQueue*>(queue_name, shm_fd)" << std::endl;
				throw std::exception(/*"Cannot access shared memory"*/);
			}
			else
			{
				std::cout << "SUCCESS: queue_shared_memory = my_shm_open<_RawQueue*>(queue_name, shm_fd)" << std::endl;
			}
		}

		std::cout << "DOING: EndOperation()" << std::endl;
		EndOperation();
	}

	bool BeginOperation()
	{
		return sem_wait(queue_operation_semaphore) == 0;
	}

	bool EndOperation()
	{
		return sem_post(queue_operation_semaphore) == 0;
	}

	bool TryBeginOperation()
	{
		return sem_trywait(queue_operation_semaphore) == 0;
	}

	_RawQueue* GetQueue()
	{
		return queue_shared_memory;
	}

	~IPCQueue()
	{
		close(shm_fd);
		sem_close(queue_operation_semaphore);

		shm_unlink(queue_name.c_str());
		sem_unlink(queue_name.c_str());
	}
};

#endif
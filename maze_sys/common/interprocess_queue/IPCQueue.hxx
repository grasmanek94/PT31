#ifndef IPCQUEUE_H
#define IPCQUEUE_H

#include <semaphore.h>
#include <string>
#include "QueueItem.hxx"
#include "RawQueue.hxx"

class IPCQueue
{
private:
	RawQueue* queue_shared_memory;
	sem_t* queue_operation_semaphore;
	sem_t* memory_prepare_semaphore;
	sem_t* elem_count_semaphore;
	int shm_fd;
	std::string queue_name;
	int deletion_fd_protection;

	bool Wait();
	bool Post();
	bool TryWait();

public:
	IPCQueue(const std::string& queue_name);
	~IPCQueue();

	bool Push(QueueItem* item);
	bool Pop(QueueItem* item);
	bool TryPush(QueueItem* item, bool only_try = false);
	bool TryPop(QueueItem* item);
	size_t Count() const;
};

#endif
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

	/* Why geen mutex? [maar wel memory_prepare_semaphore] :
		For inter-process synchronization, a mutex needs to be allo-
		cated   in  memory shared between these processes. Since the
		memory for such a mutex must be allocated dynamically,   the
		mutex needs to be explicitly initialized using mutex_init().

		We zijn shared memory aan t maken en zorgen ervoor dat er maar 1 creator is, dus nutteloos eh? Jup!
	*/

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
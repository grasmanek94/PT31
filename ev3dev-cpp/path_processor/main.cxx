#include <iostream>
#include <string>
#include <new>
#include <cstring>

#include "lib/DynamicGrid/DynamicGrid.hxx"
#include "lib/Environment/Environment.hxx"

template <size_t max_data_size_bytes = 2048>
class QueueItem
{
	size_t _operation_identifier;
	size_t _action_identifier;
	size_t _used_data_size;
	char _data[max_data_size_bytes];
public:
	QueueItem()
		:	_operation_identifier(0),
			_action_identifier(0),
			_used_data_size(0)
	{
		memset(_data, 0, max_data_size_bytes);
	}

	QueueItem(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data)
		:	_operation_identifier(operation_identifier), 
			_action_identifier(action_identifier),
			_used_data_size(data_size)
	{
		memcpy(_data, data, _used_data_size);
		if (max_data_size_bytes - _used_data_size > 0)
		{
			memset(_data + _used_data_size, 0, max_data_size_bytes - _used_data_size);
		}
	}

	size_t GetOperationIdentifier() const
	{
		return _operation_identifier;
	}

	size_t GetActionIdentifier() const
	{
		return _action_identifier;
	}

	size_t GetUsedDataSize() const
	{
		return _used_data_size;
	}

	void SetOperationIdentifier(size_t identifier) 
	{
		_operation_identifier = identifier;
	}

	void SetActionIdentifier(size_t identifier) 
	{
		_action_identifier = identifier;
	}

	void SetUsedDataSize(size_t size) 
	{
		_used_data_size = size;
	}

	template <typename T> T Convert()
	{
		// e.g. int* = Convert<int*>() // array of integers
		// or Convert<int*>()[0] = 5;
		return (T)_data;
	}
};

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class RawQueue
{
public:
	typedef QueueItem<max_data_size_bytes> _Item;
private:
	size_t count; // need to prevent overwrite of non-read items
	_Item* ptr_pushed;
	_Item* ptr_popped;
	_Item* items_begin;
	_Item* items_end;
	_Item* items; // Item items[queue_size];
public:
	RawQueue()
	{
		count = 0;
		memset(items, 0, sizeof(_Item)*max_items);
		ptr_pushed = &items[0];
		ptr_popped = &items[0];
		items_begin = &items[0];
		items_end = &items[max_items - 1];
	}

	inline size_t Count() const
	{
		/*if (ptr_pushed < ptr_popped)
		{
			return ptr_popped - ptr_pushed;
		}
		return ptr_popped - ptr_pushed;*/
		return count;
	}

	bool Push(_Item* item)
	{
		if (Count() == max_items)
		{
			return false;
		}

		memcpy(ptr_popped, item, sizeof(_Item));

		++count;
		if (++ptr_popped == items_end)
		{
			ptr_popped = items_begin;
		}
		return true;
	}

	bool Pop(_Item* item)
	{
		if (Count() == 0)
		{
			return false;
		}

		memcpy(item, ptr_pushed, sizeof(_Item));

		--count;
		if (++ptr_pushed == items_end)
		{
			ptr_pushed = items_begin;
		}
		return true;
	}

	static size_t GetSizeBytes()
	{
		return sizeof(RawQueue) + sizeof(_Item) * max_items - sizeof(_Item*);
	}
};

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <semaphore.h>

template <typename T>
T my_shm_create(const std::string& shm_name, size_t size, int& shm_fd)
{
	shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0777);
	if (shm_fd == -1)
	{
		return NULL;
	}

	int rtnval = ftruncate(shm_fd, size);
	if (rtnval != 0)
	{
		return NULL;
	}

	T shm_addr = (T)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_addr == MAP_FAILED)
	{
		return NULL;
	}

	return (shm_addr);
}

template <typename T>
T my_shm_open(const std::string& shm_name, int& shm_fd)
{
	shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0777);
	if (shm_fd == -1)
	{
		return NULL;
	}

	size_t size = lseek(shm_fd, 0, SEEK_END);

	T shm_addr = (T)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_addr == MAP_FAILED)
	{
		return NULL;
	}

	return (shm_addr);
}

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
		:	queue_shared_memory(NULL), 
			queue_operation_semaphore(SEM_FAILED), 
			shm_fd(0),
			queue_name(queue_name)
	{
		queue_operation_semaphore = sem_open(queue_name.c_str(), O_CREAT | O_EXCL, 0777, 1);

		if (queue_operation_semaphore == SEM_FAILED)
		{
			queue_operation_semaphore = sem_open(queue_name.c_str(), 0);
			if (queue_operation_semaphore == SEM_FAILED)
			{
				throw std::exception(/*"Cannot access semaphore"*/);
			}
		}

		BeginOperation();

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

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class PathProcessor
{
private:
	DynamicGrid* grid;
	IPCQueue<max_data_size_bytes, max_items> requestQueue;
	IPCQueue<max_data_size_bytes, max_items> calculatedQueue;

	typedef typename IPCQueue< max_data_size_bytes, max_items>::_RawQueue MyQueue;
	typedef typename MyQueue::_Item MyQueueItem;

	MyQueueItem* temp_item;
	JPS::PathVector* temp_path;
public:
	PathProcessor(const std::string& base_map = Environment::Map)
		:	grid(new DynamicGrid(base_map)),
			requestQueue("PathProcessor.requestQueue"),
			calculatedQueue("PathProcessor.calculatedQueue"),
			temp_item(new MyQueueItem()),
			temp_path(new JPS::PathVector())
	{}

	~PathProcessor()
	{
		delete grid;
		grid = NULL;

		delete temp_item;
		temp_item = NULL;

		delete temp_path;
		temp_path = NULL;
	}

	void Run()
	{
		while (true)
		{
			bool EndedOperation = false;
			if (requestQueue.TryBeginOperation())
			{
				MyQueue* q = requestQueue.GetQueue();

				if (q->Count())
				{
					if (q->Pop(temp_item))
					{
						if (temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2))
						{
							JPS::Position* pos_array = temp_item->template Convert<JPS::Position*>();
							JPS::Position start(pos_array[0]);
							JPS::Position target(pos_array[1]);

							EndedOperation = true;
							requestQueue.EndOperation();

							temp_path->clear();

							bool found = grid->JumpNavigate(start, target, *temp_path);

							temp_item->SetActionIdentifier(found);
							size_t data_size = temp_path->size() * sizeof(JPS::Position);
							memcpy(temp_item->template Convert<JPS::Position*>(), temp_path->data(), data_size);
							temp_item->SetUsedDataSize(data_size);

							calculatedQueue.BeginOperation();
							calculatedQueue.GetQueue()->Push(temp_item);
							calculatedQueue.EndOperation();
						}
					}
				}
			}
			if (!EndedOperation)
			{
				requestQueue.EndOperation();
			}
		}
	}
};

typedef PathProcessor<2048, 128> MyProcessor;

int main()
{
	MyProcessor* processor = new MyProcessor();

	processor->Run();

	delete processor;

	return 0;
}

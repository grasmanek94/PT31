#ifndef RAWQUEUE_H
#define RAWQUEUE_H

#include <cstring>

#include "QueueItem.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class RawShmQueue
{
public:
	typedef QueueItem<max_data_size_bytes> _Item;
private:
	size_t pushed;
	size_t popped;
	_Item  items[max_items];

	void CountDecrease(sem_t* sem)
	{
		sem_wait(sem);
	}

	void CountIncrease(sem_t* sem)
	{
		sem_post(sem);
	}

public:
	RawShmQueue()
	{
		memset(this, 0, sizeof(RawShmQueue));
	}

	size_t Count(sem_t* sem) const
	{
		int count = 0;
		sem_getvalue(sem, &count);
		return (size_t)count;
	}

	bool Push(_Item* item, sem_t* sem)
	{
		if (Count(sem) == max_items)
		{
			return false;
		}

		memcpy((void*)&items[pushed], item, sizeof(_Item));

		CountIncrease(sem);
		if (++pushed == max_items)
		{
			pushed = 0;
		}
		return true;
	}

	bool Pop(_Item* item, sem_t* sem)
	{
		if (Count(sem) == 0)
		{
			return false;
		}

		memcpy(item, (void*)&items[popped], sizeof(_Item));

		CountDecrease(sem);
		if (++popped == max_items)
		{
			popped = 0;
		}
		return true;
	}

	static size_t GetSizeBytes()
	{
		return sizeof(RawShmQueue);
	}

	static size_t MaxCount()
	{
		return max_items;
	}

	static size_t MaxBytesPerElem()
	{
		return max_data_size_bytes;
	}
};

#endif
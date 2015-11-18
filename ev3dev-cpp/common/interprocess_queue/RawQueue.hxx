#ifndef RAWQUEUE_H
#define RAWQUEUE_H

#include <cstring>
#include <algorithm>

#include "QueueItem.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class RawQueue
{
public:
	typedef QueueItem<max_data_size_bytes> _Item;
private:
	size_t count;
	size_t pushed;
	size_t popped;
	_Item  items[max_items];
public:
	RawQueue()
	{
		memset(this, 0, sizeof(RawQueue));
	}

	inline size_t Count() const
	{
		return count;
	}

	bool Push(_Item* item)
	{
		if (Count() == max_items)
		{
			return false;
		}

		memcpy(&item[popped], item, sizeof(_Item));

		++count;

		if (++popped == max_items)
		{
			popped = 0;
		}
		return true;
	}

	bool Pop(_Item* item)
	{
		if (Count() == 0)
		{
			return false;
		}

		memcpy(item, &items[pushed], sizeof(_Item));

		--count;
		if (++pushed == max_items)
		{
			pushed = 0;
		}
		return true;
	}

	static size_t GetSizeBytes()
	{
		return sizeof(RawQueue);
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
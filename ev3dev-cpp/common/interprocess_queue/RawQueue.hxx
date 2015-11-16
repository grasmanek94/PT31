#ifndef RAWQUEUE_H
#define RAWQUEUE_H

#include <cstring>

#include "QueueItem.hxx"

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

#endif
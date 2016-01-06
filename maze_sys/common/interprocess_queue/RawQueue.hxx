#ifndef RAWQUEUE_H
#define RAWQUEUE_H

#include "QueueItem.hxx"

static const size_t max_items_for_RawQueue = 128;

class RawQueue
{
private:
	size_t count;
	size_t pushed;
	size_t popped;
	QueueItem items[max_items_for_RawQueue];

public:
	RawQueue();
	size_t Count() const;
	bool Push(QueueItem* item);
	bool Pop(QueueItem* item);
	static size_t GetSizeBytes();
	static size_t MaxCount();
	static size_t MaxBytesPerElem();
};

#endif
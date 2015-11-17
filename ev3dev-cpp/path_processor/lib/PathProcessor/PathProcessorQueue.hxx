#ifndef PATHPROCESSORQUEUE_H
#define PATHPROCESSORQUEUE_H

#include <interprocess_queue/IPCQueue.hxx>

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class PathProcessorQueue
{
public:
	typedef IPCQueue< max_data_size_bytes, max_items> MyIPCQueue;
	typedef typename MyIPCQueue::_RawQueue MyRawQueue;
	typedef typename MyRawQueue::_Item MyQueueItem;

private:

	MyIPCQueue requestQueue;
	MyIPCQueue calculatedQueue;

public:

	PathProcessorQueue()
		:	requestQueue("PathProcessor.requestQueue"),
			calculatedQueue("PathProcessor.calculatedQueue")
	{ }

	MyIPCQueue& Request()
	{
		return requestQueue;
	}

	MyIPCQueue& Calculated()
	{
		return calculatedQueue;
	}
};
#endif
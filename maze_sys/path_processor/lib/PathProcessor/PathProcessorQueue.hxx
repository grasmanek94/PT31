#ifndef PATHPROCESSORQUEUE_H
#define PATHPROCESSORQUEUE_H

#include <interprocess_queue/IPCQueue.hxx>

class PathProcessorQueue
{
private:

	IPCQueue requestQueue;
	IPCQueue calculatedQueue;

public:

	PathProcessorQueue();
	IPCQueue* Request();
	IPCQueue* Calculated();
};
#endif
#include <interprocess_queue/IPCQueue.hxx>
#include "PathProcessorQueue.hxx"

PathProcessorQueue::PathProcessorQueue()
	:	requestQueue("PathProcessor.requestQueue"),
		calculatedQueue("PathProcessor.calculatedQueue")
{ }

IPCQueue* PathProcessorQueue::Request()
{
	return &requestQueue;
}

IPCQueue* PathProcessorQueue::Calculated()
{
	return &calculatedQueue;
}
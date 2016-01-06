#ifndef PATHPROCESSOR_H
#define PATHPROCESSOR_H

#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include "DynamicGrid/DynamicGrid.hxx"
#include "Environment/Environment.hxx"
#include "PathProcessorQueue.hxx"

class PathProcessor
{
private:
	DynamicGrid* grid;
	PathProcessorQueue queues;
	QueueItem* temp_item;
	JPS::PathVector* temp_path;

	IPCQueue* request_queue;
	IPCQueue* calculated_queue;

	bool running;
	bool donerunning;
public:
	PathProcessor(const std::string& base_map = Environment::Map);
	~PathProcessor();
	void Run();
	void Stop(bool wait = true);
};

#endif
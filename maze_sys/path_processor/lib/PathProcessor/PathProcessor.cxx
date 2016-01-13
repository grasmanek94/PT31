#include <iostream>
#include <string>

#include <unistd.h>

#include <JumpPointSearch/JPS.hxx>
#include "DynamicGrid/DynamicGrid.hxx"
#include "Environment/Environment.hxx"
#include "PathProcessorQueue.hxx"
#include "PathProcessor.hxx"

PathProcessor::PathProcessor(const std::string& base_map)
	:	grid(new DynamicGrid(base_map)),
		queues(),
		temp_item(new QueueItem()),
		temp_path(new JPS::PathVector()),
		running(true),
		donerunning(false)
{}

PathProcessor::~PathProcessor()
{
	delete grid;
	grid = NULL;

	delete temp_item;
	temp_item = NULL;

	delete temp_path;
	temp_path = NULL;
}

void PathProcessor::Run()
{
	request_queue = queues.Request();
	calculated_queue = queues.Calculated();

	while (running)
	{
		if (request_queue->Pop(temp_item) && temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2))
		{
			JPS::Position* pos_array = temp_item->template Convert<JPS::Position*>();
			JPS::Position start(pos_array[0]);
			JPS::Position target(pos_array[1]);

			temp_path->clear();

			bool found = grid->JumpNavigate(start, target, *temp_path);
			//bool found = grid->FullNavigate(start, target, *temp_path);

			temp_item->SetActionIdentifier(found);

			size_t data_size = temp_path->size() * sizeof(JPS::Position);
			temp_item->Assign(temp_path->data(), data_size);

			calculated_queue->Push(temp_item);

			//if (found)
			//{
			//	std::cout << grid->ToASCII(temp_item->template Convert<JPS::Position*>(), data_size / sizeof(JPS::Position)) << std::endl;
			//}
			//else
			//{
			//	std::cout << "NOT FOUND" << std::endl;
			//}
		}
	}
	donerunning = true;
}

void PathProcessor::Stop(bool wait)
{
	running = false;
	while (wait && !donerunning)
	{
		sleep(1);
	}
}
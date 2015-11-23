#ifndef PATHPROCESSOR_H
#define PATHPROCESSOR_H

#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include "DynamicGrid/DynamicGrid.hxx"
#include "Environment/Environment.hxx"
#include "PathProcessorQueue.hxx"

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class PathProcessor
{
private:
	typedef PathProcessorQueue<max_data_size_bytes, max_items> PPQ;
	typedef typename PPQ::MyRawQueue MyQueue;
	typedef typename PPQ::MyQueueItem MyQueueItem;

	DynamicGrid* grid;
	PPQ queues;
	MyQueueItem* temp_item;
	JPS::PathVector* temp_path;

	typename PPQ::MyIPCQueue* request_queue;
	typename PPQ::MyIPCQueue* calculated_queue;

	bool running;
	bool donerunning;
public:
	PathProcessor(const std::string& base_map = Environment::Map)
		:	grid(new DynamicGrid(base_map)),
			queues(),
			temp_item(new MyQueueItem()),
			temp_path(new JPS::PathVector()),
			running(true),
			donerunning(false)
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
		request_queue = queues.Request();
		calculated_queue = queues.Calculated();

		while (running)
		{
			request_queue->Wait();
			bool pop_result = request_queue->Pop(temp_item);
			request_queue->Post();

			if (pop_result && temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2))
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

				calculated_queue->Wait();
				calculated_queue->Push(temp_item);
				calculated_queue->Post();

				if (found)
				{
					std::cout << grid->ToASCII(temp_item->template Convert<JPS::Position*>(), data_size / sizeof(JPS::Position)) << std::endl;
				}
				else
				{
					std::cout << "NOT FOUND" << std::endl;
				}
			}
		}
		donerunning = true;
	}

	void Stop(bool wait = true)
	{
		running = false;
		while (wait && !donerunning)
		{
			usleep(100);
		}
	}
};

#endif
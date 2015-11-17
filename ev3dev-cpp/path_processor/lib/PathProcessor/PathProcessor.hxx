#ifndef PATHPROCESSOR_H
#define PATHPROCESSOR_H

#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include "lib/DynamicGrid/DynamicGrid.hxx"
#include "lib/Environment/Environment.hxx"
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

public:
	PathProcessor(const std::string& base_map = Environment::Map)
		:	grid(new DynamicGrid(base_map)),
			queues(),
			temp_item(new MyQueueItem()),
			temp_path(new JPS::PathVector())
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
		while (true)
		{
			std::cout << "TRYING: queues.Request().BeginOperation()" << std::endl;
			bool EndedOperation = false;
			if (queues.Request().BeginOperation())
			{
				std::cout << "SUCCESS: queues.Request().BeginOperation()" << std::endl;
				MyQueue* q = queues.Request().GetQueue();

				if (q->Count())
				{
					std::cout << "SUCCESS: q->Count()" << std::endl;
					if (q->Pop(temp_item))
					{
						std::cout << "SUCCESS: q->Pop(temp_item)" << std::endl;
						if (temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2))
						{
							std::cout << "SUCCESS: temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2)" << std::endl;
							JPS::Position* pos_array = temp_item->template Convert<JPS::Position*>();
							JPS::Position start(pos_array[0]);
							JPS::Position target(pos_array[1]);

							EndedOperation = true;
							std::cout << "DOING: queues.Request().EndOperation()" << std::endl;
							queues.Request().EndOperation();

							temp_path->clear();

							std::cout << "DOING: grid->JumpNavigate(start, target, *temp_path)" << std::endl;
							bool found = grid->JumpNavigate(start, target, *temp_path);

							temp_item->SetActionIdentifier(found);
							size_t data_size = temp_path->size() * sizeof(JPS::Position);
							memcpy(temp_item->template Convert<JPS::Position*>(), temp_path->data(), data_size);
							temp_item->SetUsedDataSize(data_size);

							std::cout << "DOING: queues.Calculated().BeginOperation()" << std::endl;
							queues.Calculated().BeginOperation();
							std::cout << "DOING: queues.Calculated().GetQueue()->Push(temp_item)" << std::endl;
							queues.Calculated().GetQueue()->Push(temp_item);
							std::cout << "DOING: queues.Calculated().EndOperation()" << std::endl;
							queues.Calculated().EndOperation();
						}
						else
						{
							std::cout << "FAILED: temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2)" << std::endl;
						}
					}
					else
					{
						std::cout << "FAILED: q->Pop(temp_item)" << std::endl;
					}
				}
				else
				{
					std::cout << "FAILED: q->Count()" << std::endl;
				}
			}
			else
			{
				std::cout << "FAILED: queues.Request().BeginOperation()" << std::endl;
			}
			if (!EndedOperation)
			{
				std::cout << "DOING: queues.Request().EndOperation()" << std::endl;
				queues.Request().EndOperation();
				std::cout << "DONE: queues.Request().EndOperation()" << std::endl;
			}
			sleep(1);
		}
	}
};

#endif
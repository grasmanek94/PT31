#ifndef PATHPROCESSOR_H
#define PATHPROCESSOR_H

#include "lib/DynamicGrid/DynamicGrid.hxx"
#include "lib/Environment/Environment.hxx"
#include <interprocess_queue/IPCQueue.hxx>

template <size_t max_data_size_bytes = 2048, size_t max_items = 128>
class PathProcessor
{
private:
	DynamicGrid* grid;
	IPCQueue<max_data_size_bytes, max_items> requestQueue;
	IPCQueue<max_data_size_bytes, max_items> calculatedQueue;

	typedef typename IPCQueue< max_data_size_bytes, max_items>::_RawQueue MyQueue;
	typedef typename MyQueue::_Item MyQueueItem;

	MyQueueItem* temp_item;
	JPS::PathVector* temp_path;
public:
	PathProcessor(const std::string& base_map = Environment::Map)
		: grid(new DynamicGrid(base_map)),
		requestQueue("PathProcessor.requestQueue"),
		calculatedQueue("PathProcessor.calculatedQueue"),
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
			bool EndedOperation = false;
			if (requestQueue.TryBeginOperation())
			{
				MyQueue* q = requestQueue.GetQueue();

				if (q->Count())
				{
					if (q->Pop(temp_item))
					{
						if (temp_item->GetUsedDataSize() >= (sizeof(JPS::Position) * 2))
						{
							JPS::Position* pos_array = temp_item->template Convert<JPS::Position*>();
							JPS::Position start(pos_array[0]);
							JPS::Position target(pos_array[1]);

							EndedOperation = true;
							requestQueue.EndOperation();

							temp_path->clear();

							bool found = grid->JumpNavigate(start, target, *temp_path);

							temp_item->SetActionIdentifier(found);
							size_t data_size = temp_path->size() * sizeof(JPS::Position);
							memcpy(temp_item->template Convert<JPS::Position*>(), temp_path->data(), data_size);
							temp_item->SetUsedDataSize(data_size);

							calculatedQueue.BeginOperation();
							calculatedQueue.GetQueue()->Push(temp_item);
							calculatedQueue.EndOperation();
						}
					}
				}
			}
			if (!EndedOperation)
			{
				requestQueue.EndOperation();
			}
		}
	}
};

#endif
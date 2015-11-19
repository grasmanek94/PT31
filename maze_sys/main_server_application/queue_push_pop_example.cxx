#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include <DynamicGrid/DynamicGrid.hxx>
#include <Environment/Environment.hxx>

typedef PathProcessorQueue<> PPQ;
PPQ queues;

int main()
{
	PPQ::MyIPCQueue* request_queue = queues.Request();
	PPQ::MyIPCQueue* calculated_queue = queues.Calculated();

	PPQ::MyQueueItem item;
	JPS::Position* arr = item.template Convert<JPS::Position*>();

	arr[0] = JPS::Pos(2, 2);
	arr[1] = JPS::Pos(30, 30);

	item.SetUsedDataSize(sizeof(JPS::Position) * 2);

	request_queue->Wait();
	bool push_result = request_queue->Push(&item);
	request_queue->Post();

	if (push_result)
	{
		while (true)
		{
			calculated_queue->Wait();
			bool pop_result = calculated_queue->Pop(&item);
			calculated_queue->Post();

			if (pop_result)
			{
				size_t elems = (item.GetUsedDataSize() / sizeof(JPS::Position));
				JPS::Position* calculated_positions = item.template Convert<JPS::Position*>();

				if (elems)
				{
					std::cout << DynamicGrid(Environment::Map).ToASCII(calculated_positions, elems) << std::endl;
				}
				else
				{
					std::cout << "NOT FOUND" << std::endl;
				}
				break;
			}
		}
	}

	return 0;
}
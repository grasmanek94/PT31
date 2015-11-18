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
	size_t rq_size = request_queue->Count();

	request_queue->Post();

	if (push_result)
	{
		std::cout << "PUSHED, SIZE: " << rq_size << std::endl;
		std::cout << item << std::endl;
			
		while (true)
		{
			request_queue->Wait();
			calculated_queue->Wait();

			std::cout << "R SIZE: " << request_queue->Count() << std::endl;
			std::cout << "C SIZE: " << calculated_queue->Count() << std::endl;
			bool pop_result = calculated_queue->Pop(&item);

			calculated_queue->Post();
			request_queue->Post();

			if (pop_result)
			{
				size_t size = item.GetUsedDataSize();
				size_t elems = (size / sizeof(JPS::Position));

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

			sleep(5);
		}
	}

	return 0;
}
#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include <DynamicGrid/DynamicGrid.hxx>
#include <Environment/Environment.hxx>

PathProcessorQueue queues;

int __main()
{
	IPCQueue* request_queue = queues.Request();
	IPCQueue* calculated_queue = queues.Calculated();

	QueueItem item;
	JPS::Position* arr = item.template Convert<JPS::Position*>();

	arr[0] = JPS::Pos(2, 2);
	arr[1] = JPS::Pos(30, 20);

	item.SetUsedDataSize(sizeof(JPS::Position) * 2);

	if (request_queue->Push(&item))
	{
		while (true)
		{
			if (calculated_queue->Pop(&item))
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
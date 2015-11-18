#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>

typedef PathProcessorQueue<> PPQ;
PPQ ppq;

int main()
{
	std::cout << "TRYING: ppq.Request().BeginOperation()" << std::endl;
	if (ppq.Request().BeginOperation())
	{
		std::cout << "SUCCESS: ppq.Request().BeginOperation()" << std::endl;

		std::cout << "DOING: PPQ::MyRawQueue* q = ppq.Request().GetQueue()" << std::endl;
		PPQ::MyRawQueue* req_q = ppq.Request().GetQueue();

		std::cout << "DOING: PPQ::MyQueueItem item" << std::endl;
		PPQ::MyQueueItem item;

		std::cout << "DOING: JPS::Position* arr = item.template Convert<JPS::Position*>()" << std::endl;
		JPS::Position* arr = item.template Convert<JPS::Position*>();

		std::cout << "DOING: arr[0] = JPS::Pos(1, 1)" << std::endl;
		arr[0] = JPS::Pos(1, 1);

		std::cout << "DOING: arr[1] = JPS::Pos(32, 32)" << std::endl;
		arr[1] = JPS::Pos(32, 32);

		std::cout << "DOING: item.SetUsedDataSize(sizeof(JPS::Position) * 2)" << std::endl;
		item.SetUsedDataSize(sizeof(JPS::Position) * 2);

		std::cout << "TRYING: req_q->Push(&item)" << std::endl;
		if (req_q->Push(&item))
		{
			std::cout << "SUCCESS: req_q->Push(&item)" << std::endl;
			ppq.Request().EndOperation();

			while (true)
			{
				if (ppq.Calculated().BeginOperation())
				{
					std::cout << "SUCCESS: ppq.Calculated().BeginOperation()" << std::endl;
					PPQ::MyRawQueue* calc_q = ppq.Calculated().GetQueue();

					if (calc_q->Count())
					{
						std::cout << "SUCCESS: calc_q->Count()" << std::endl;
						if (calc_q->Pop(&item))
						{
							size_t size = item.GetUsedDataSize();
							size_t elems = (size / sizeof(JPS::Position));
							std::cout << "SUCCESS: c->Pop(&item)" << std::endl;
							std::cout << "SIZE: " << size << " bytes (ELEMENTS: " << elems << " )" << std::endl;

							JPS::Position* calculated_positions = item.template Convert<JPS::Position*>();
							for (size_t i = 0; i < elems; ++i)
							{
								std::cout << "Element " << i << " POS:(" << calculated_positions[i].x << "," << calculated_positions[i].y << ")" << std::endl;
							}
							break;
						}
						else
						{
							std::cout << "FAILED: calc_q->Pop(&item)" << std::endl;
						}				
					}
					else
					{
						std::cout << "FAILED: c->Count()" << std::endl;
					}
					ppq.Calculated().EndOperation();
				}
				else
				{
					std::cout << "FAILED: ppq.Calculated().BeginOperation()" << std::endl;
				}
				sleep(5);
			}
		}
		else
		{
			std::cout << "FAILED: req_q->Push(&item)" << std::endl;
			ppq.Request().EndOperation();
		}
	}
	else
	{		
		std::cout << "FAILED: PPQ.Request().BeginOperation()" << std::endl;
	}
	return 0;
}
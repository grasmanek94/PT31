#include <iostream>
#include <string>

#include <JumpPointSearch/JPS.hxx>
#include <PathProcessor/PathProcessorQueue.hxx>
#include <DynamicGrid/DynamicGrid.hxx>
#include <Environment/Environment.hxx>

typedef PathProcessorQueue<> PPQ;
PPQ ppq;

int main()
{
	if (ppq.Request().BeginOperation())
	{
		PPQ::MyRawQueue* req_q = ppq.Request().GetQueue();

		PPQ::MyQueueItem item;

		JPS::Position* arr = item.template Convert<JPS::Position*>();

		arr[0] = JPS::Pos(2, 2);

		arr[1] = JPS::Pos(30, 30);

		item.SetUsedDataSize(sizeof(JPS::Position) * 2);

		if (req_q->Push(&item))
		{
			std::cout << "SUCCESS: req_q->Push(&item)" << std::endl;
			std::cout << item << std::endl;
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
							if (elems)
							{
								std::cout << DynamicGrid(Environment::Map).ToASCII(calculated_positions, elems) << std::endl;
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
				sleep(5);
			}
		}
		else
		{
			std::cout << "FAILED: req_q->Push(&item)" << std::endl;
			ppq.Request().EndOperation();
		}
	}
	return 0;
}
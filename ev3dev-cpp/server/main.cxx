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
		PPQ::MyRawQueue* q = ppq.Request().GetQueue();

		PPQ::MyQueueItem item;	
		JPS::Position* arr = item.template Convert<JPS::Position*>();
		arr[0] = JPS::Pos(1, 1);
		arr[1] = JPS::Pos(32, 32);
		item.SetUsedDataSize(sizeof(JPS::Position) * 2);

		if (q->Push(&item))
		{
			std::cout << "SUCCESS: q->Push(&item)" << std::endl;
			ppq.Request().EndOperation();

			while (true)
			{
				if (ppq.Calculated().BeginOperation())
				{
					std::cout << "SUCCESS: ppq.Calculated().BeginOperation()" << std::endl;
					PPQ::MyRawQueue* c = ppq.Calculated().GetQueue();

					if (c->Count())
					{
						std::cout << "SUCCESS: c->Count()" << std::endl;
						if (c->Pop(&item))
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
							std::cout << "FAILED: c->Pop(&item)" << std::endl;
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
				sleep(1);
			}
		}
		else
		{
			std::cout << "FAILED: q->Push(&item)" << std::endl;
			ppq.Request().EndOperation();
		}
	}
	else
	{		
		std::cout << "FAILED: PPQ.Request().BeginOperation()" << std::endl;
		ppq.Request().EndOperation();
	}
	return 0;
}
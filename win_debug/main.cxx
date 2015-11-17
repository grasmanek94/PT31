#include "../ev3dev-cpp/common/interprocess_queue/QueueItem.hxx"
#include "../ev3dev-cpp/common/interprocess_queue/RawQueue.hxx"

namespace RawQueueDebug
{
	typedef RawQueue<> RQ;
	typedef RQ::_Item Item;


	void VisualStudioDebugRawQueue()
	{
		{
			RQ* queue;

			std::cout << RQ::GetSizeBytes() << std::endl;
			queue = (RQ*)new char[RQ::GetSizeBytes()];
			new (queue) RQ();
			Item item;
			item.SetUsedDataSize(14);
			char* arr = item.template Convert<char*>();
			arr[0] = 'R';
			arr[1] = 'a';
			arr[2] = 'f';
			arr[3] = 'a';
			arr[4] = 'l';
			arr[5] = ' ';
			arr[6] = 'G';
			arr[7] = 'r';
			arr[8] = 'a';
			arr[9] = 's';
			arr[10] = 'm';
			arr[11] = 'a';
			arr[12] = 'n';
			arr[13] = 0;

			for (int i = 0; i < queue->MaxCount(); ++i)
			{
				if (!queue->Push(&item))
				{
					std::cout << "?" << std::endl;
				}
			}

			if (!queue->Push(&item))
			{
				std::cout << "WORKS" << std::endl;
			}

			Item item2;

			queue->Pop(&item2);

			std::cout << item2.template Convert<char*>() << std::endl;//should print "Rafa³ Grasman"

			queue->Push(&item);

			queue->Pop(&item2);
			queue->Push(&item);
			queue->Pop(&item2);
			queue->Push(&item);
			queue->Pop(&item2);
			queue->Push(&item);
			queue->Pop(&item2);
			queue->Push(&item);

			delete queue;// set bp here
		}
		{
			RQ* queue2;

			std::cout << RQ::GetSizeBytes() << std::endl;
			queue2 = (RQ*)new char[RQ::GetSizeBytes()];
			new (queue2) RQ();
			Item item;
			item.SetUsedDataSize(14);
			char* arr = item.template Convert<char*>();
			arr[0] = 'R';
			arr[1] = 'a';
			arr[2] = 'f';
			arr[3] = 'a';
			arr[4] = 'l';
			arr[5] = ' ';
			arr[6] = 'G';
			arr[7] = 'r';
			arr[8] = 'a';
			arr[9] = 's';
			arr[10] = 'm';
			arr[11] = 'a';
			arr[12] = 'n';
			arr[13] = 0;

			for (int i = 0; i < 10; ++i)
			{
				if (!queue2->Push(&item))
				{
					std::cout << "?" << std::endl;
				}
			}

			Item item2;

			queue2->Pop(&item2);
			queue2->Pop(&item2);
			queue2->Pop(&item2);
			queue2->Pop(&item2);
				 
			queue2->Push(&item);
			queue2->Push(&item);
			queue2->Push(&item);
				 
			queue2->Pop(&item2);
			queue2->Pop(&item2);
			queue2->Pop(&item2);
			queue2->Pop(&item2);

			delete queue2;// set bp here
		}// set bp here

	} 
};

int main()
{
	RawQueueDebug::VisualStudioDebugRawQueue();
	return 0;
}
#include "../ev3dev-cpp/common/interprocess_queue/QueueItem.hxx"
#include "../ev3dev-cpp/common/interprocess_queue/RawQueue.hxx"

namespace RawQueueDebug
{
	typedef RawQueue<> RQ;
	typedef RQ::_Item Item;

	RQ* queue;

	void VisualStudioDebugRawQueue()
	{
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
	} // set bp here
};

int main()
{
	RawQueueDebug::VisualStudioDebugRawQueue();
	return 0;
}
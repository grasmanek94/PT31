#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <cstring>
#include <iostream>

static const size_t max_data_size_bytes_for_QueueItem = 2048;

class QueueItem
{
private:
	size_t _operation_identifier;
	size_t _action_identifier;
	size_t _used_data_size;
	char _data[max_data_size_bytes_for_QueueItem];

	bool UpdateSize(size_t size);
public:
	
	QueueItem();
	QueueItem(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data);
	void ReInit(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data);

	size_t GetOperationIdentifier() const;
	size_t GetActionIdentifier() const;
	size_t GetUsedDataSize() const;
	void SetOperationIdentifier(size_t identifier);
	void SetActionIdentifier(size_t identifier);
	bool SetUsedDataSize(size_t size, size_t begin = 0);
	template <typename T> T Convert(size_t begin = 0)
	{
		// e.g. int* = Convert<int*>() // array of integers
		// or Convert<int*>()[0] = 5;
		return (T)(_data + begin);
	}

	template <typename T> void* Assign(T data, size_t size, size_t begin = 0)
	{
		SetUsedDataSize(size, begin);
		return memcpy(_data + begin, data, size);
	}

	friend std::ostream& operator<<(std::ostream& os, const QueueItem& dt);
};

std::ostream& operator<<(std::ostream& os, const QueueItem& dt);
#endif
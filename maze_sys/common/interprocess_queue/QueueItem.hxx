#ifndef QUEUEITEM_H
#define QUEUEITEM_H

#include <cstring>
#include <iostream>

template <size_t max_data_size_bytes = 2048>
class QueueItem
{
	size_t _operation_identifier;
	size_t _action_identifier;
	size_t _used_data_size;
	char _data[max_data_size_bytes];

	typedef QueueItem<max_data_size_bytes> CurrentQueueItem;
public:
	
	QueueItem()
		: _operation_identifier(0),
		_action_identifier(0),
		_used_data_size(0)
	{
		memset(_data, 0, max_data_size_bytes);
	}

	QueueItem(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data)
	{
		ReInit(operation_identifier, action_identifier, data_size, data);
	}

	void ReInit(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data)
	{
		_operation_identifier = operation_identifier;
		_action_identifier = action_identifier;
		_used_data_size = data_size;
		if (_used_data_size > max_data_size_bytes)
		{
			_used_data_size = max_data_size_bytes;
			// should really throw here....?
		}

		memcpy(_data, data, _used_data_size);
		if (max_data_size_bytes - _used_data_size > 0)
		{
			memset(_data + _used_data_size, 0, max_data_size_bytes - _used_data_size);
		}
	}

	size_t GetOperationIdentifier() const
	{
		return _operation_identifier;
	}

	size_t GetActionIdentifier() const
	{
		return _action_identifier;
	}

	size_t GetUsedDataSize() const
	{
		return _used_data_size;
	}

	void SetOperationIdentifier(size_t identifier)
	{
		_operation_identifier = identifier;
	}

	void SetActionIdentifier(size_t identifier)
	{
		_action_identifier = identifier;
	}

	void SetUsedDataSize(size_t size)
	{
		_used_data_size = size;
	}

	template <typename T> T Convert(size_t begin = 0)
	{
		// e.g. int* = Convert<int*>() // array of integers
		// or Convert<int*>()[0] = 5;
		return (T)(&_data[begin]);
	}

	template <typename T> void* Assign(T data, size_t size)
	{
		SetUsedDataSize(size);
		return memcpy(this->template Convert<T>(), data, size);
	}

	template <size_t s>
	friend std::ostream& operator<<(std::ostream& os, const QueueItem<s>& dt);
};

template <size_t max_data_size_bytes>
std::ostream& operator<<(std::ostream& os, const QueueItem<max_data_size_bytes>& dt)
{
	os << "O[" << dt._operation_identifier << "] A[" << dt._action_identifier << "] S[" << dt._used_data_size << "] D[ ";
	for (size_t i = 0; i < 32; ++i)
	{
		os << std::hex << (int)dt._data[i] << " ";
	}
	os << "]";
	return os;
}
#endif
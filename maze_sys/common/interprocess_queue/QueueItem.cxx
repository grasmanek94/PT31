
#include <cstring>
#include <iostream>
#include "QueueItem.hxx"

bool QueueItem::UpdateSize(size_t size)
{
	_used_data_size = size;
	if (_used_data_size > max_data_size_bytes_for_QueueItem)
	{
		_used_data_size = max_data_size_bytes_for_QueueItem;
		return false;
	}
	return true;
}

QueueItem::QueueItem()
	: _operation_identifier(0),
	_action_identifier(0),
	_used_data_size(0)
{
	memset(_data, 0, max_data_size_bytes_for_QueueItem);
}

QueueItem::QueueItem(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data)
{
	ReInit(operation_identifier, action_identifier, data_size, data);
}

void QueueItem::ReInit(size_t operation_identifier, size_t action_identifier, size_t data_size, void* data)
{
	_operation_identifier = operation_identifier;
	_action_identifier = action_identifier;
	UpdateSize(data_size);

	memcpy(_data, data, _used_data_size);
	if (max_data_size_bytes_for_QueueItem - _used_data_size > 0)
	{
		memset(_data + _used_data_size, 0, max_data_size_bytes_for_QueueItem - _used_data_size);
	}
}

size_t QueueItem::GetOperationIdentifier() const
{
	return _operation_identifier;
}

size_t QueueItem::GetActionIdentifier() const
{
	return _action_identifier;
}

size_t QueueItem::GetUsedDataSize() const
{
	return _used_data_size;
}

void QueueItem::SetOperationIdentifier(size_t identifier)
{
	_operation_identifier = identifier;
}

void QueueItem::SetActionIdentifier(size_t identifier)
{
	_action_identifier = identifier;
}

bool QueueItem::SetUsedDataSize(size_t size, size_t begin)
{
	return UpdateSize(begin + size);
}

std::ostream& operator<<(std::ostream& os, const QueueItem& dt)
{
	os << "O[" << dt._operation_identifier << "] A[" << dt._action_identifier << "] S[" << dt._used_data_size << "] D[ ";
	for (size_t i = 0; i < 32; ++i)
	{
		os << std::hex << (int)dt._data[i] << " ";
	}
	os << "]";
	return os;
}
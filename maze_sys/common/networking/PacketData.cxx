#include "PacketData.hxx"

PacketData::PacketData()
	:	currentOffset_r(0)
{ }

const uint8_t* PacketData::Serialize() const
{
	return _data.data();
}

const uint8_t* PacketData::SerializeFromReadOffset() const
{
	return _data.data() + currentOffset_r;
}

void PacketData::Deserialize(uint8_t* data, size_t size)
{
	_data.assign(data, data + size);
	currentOffset_r = 0;
}

size_t PacketData::size()
{
	return _data.size();
}

size_t PacketData::remaining_size()
{
	return _data.size() - currentOffset_r;
}
void PacketData::ResetReader()
{
	currentOffset_r = 0;
}
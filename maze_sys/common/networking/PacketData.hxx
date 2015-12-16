#ifndef PACKETDATA_H
#define PACKETDATA_H

#include <vector>
#include <string>
#include <cstring>

struct SpecifySize
{
	uint8_t* data;
	//specify data size or MAX size here
	size_t size;
};

class PacketData
{
public:
	template <typename T>
	friend PacketData& operator<<(PacketData& stream, T &data);
	template <typename T>
	friend PacketData& operator>>(PacketData& stream, T &data);

private:
	std::vector<uint8_t> _data;
	size_t currentOffset_r;

	//for std::string
	template <typename T = std::string>
	PacketData& in_stream_impl(std::string &data)
	{
		size_t data_size = data.size();
		(*this) << data_size;

		_data.insert(_data.end(), reinterpret_cast<const uint8_t*>(data.data()), reinterpret_cast<const uint8_t*>(data.data()) + data_size);

		return *this;
	}

	template <typename T = std::string>
	PacketData& out_stream_impl(std::string &data)
	{
		size_t total_string_size;

		(*this) >> total_string_size;

		char* tmp_string = new char[total_string_size + 1];

		memcpy(tmp_string, _data.data() + currentOffset_r, total_string_size);
		//std::copy(_data.data() + currentOffset_r, _data.data() + currentOffset_r + total_string_size, tmp_string);
		currentOffset_r += total_string_size;

		tmp_string[total_string_size] = 0;
		data.assign(tmp_string);

		delete[] tmp_string;

		return *this;
	}

	//for std::vector<T>
	template <typename T>
	PacketData& in_stream_impl(std::vector<T> &data)
	{
		size_t data_size = data.size();
		(*this) << data_size;

		_data.insert(_data.end(), reinterpret_cast<const uint8_t*>(data.data()), reinterpret_cast<const uint8_t*>(data.data()) + (data_size * sizeof(T)));

		return *this;
	}

	template <typename T>
	PacketData& out_stream_impl(std::vector<T> &data)
	{
		size_t total_vector_size;

		(*this) >> total_vector_size;

		T* tmp_data = new T[total_vector_size];
		size_t total_data_size = total_vector_size * sizeof(T);

		memcpy(tmp_data, _data.data() + currentOffset_r, total_data_size);
		currentOffset_r += total_data_size;

		data.assign(tmp_data, tmp_data + total_vector_size);

		delete[] tmp_data;

		return *this;
	}

	//for SpecifySize
	template <typename T = SpecifySize>
	PacketData& in_stream_impl(SpecifySize &data)
	{
		(*this) << data.size;
		_data.insert(_data.end(), reinterpret_cast<uint8_t*>(data.data), reinterpret_cast<uint8_t*>(data.data) + data.size);
		return *this;
	}

	template <typename T = SpecifySize>
	PacketData& out_stream_impl(SpecifySize &data)
	{
		size_t total_size;

		(*this) >> total_size;

		memcpy(data.data, _data.data() + currentOffset_r, total_size > data.size ? data.size : total_size);
		currentOffset_r += data.size;

		data.size = total_size;

		return *this;
	}

	//Please keep this ones as LAST, else any specialization/overrides will be overriden by these below
	template <typename T>
	PacketData& in_stream_impl(T &data)
	{
		_data.insert(_data.end(), reinterpret_cast<uint8_t*>(&data), reinterpret_cast<uint8_t*>(&data) + sizeof(data));
		return *this;
	}
	
	template <typename T>
	PacketData& out_stream_impl(T &data)
	{
		memcpy(&data, _data.data() + currentOffset_r, sizeof(data));
		currentOffset_r += sizeof(data);
		return *this;
	}

public:

	PacketData();
	const uint8_t* Serialize() const;
	const uint8_t* SerializeFromReadOffset() const;

	void Deserialize(uint8_t* data, size_t size);
	size_t size();
	size_t remaining_size();
	void ResetReader();
	/*	Usage example:
		int main()
		{
			std::string A("hello world");
			std::string B;
			std::string lets_try;
			char something[32] = "what the fuck this works";
			//char lets_try[16];

			std::vector<int> C({ 10,100,55,78,66,72 });
			std::vector<int> D;

			PacketData d;

			d << A;
			d << C;
			d << SpecifySize{ (uint8_t*)something, strlen(something) };
			d >> B;
			d >> D;
			//d >> SpecifySize{ (uint8_t*)lets_try, 15 };
			//lets_try[15] = 0;
			d >> lets_try;
	
			const uint8_t* x = d.Serialize();

			std::cout << lets_try << std::endl;
			return 0;
		}	
	*/
};

template <typename T>
PacketData& operator<<(PacketData& stream, T &data)
{
	return stream.in_stream_impl(data);
}

template <typename T>
PacketData& operator>>(PacketData& stream, T &data)
{
	return stream.out_stream_impl(data);
}

#endif
#pragma once
#include <vector>
#include <string>
#include <boost/variant.hpp>

template <typename ElemT>
struct HexTo 
{
	ElemT value;
	operator ElemT() const 
	{ 
		return value; 
	}
	friend std::istream& operator>>(std::istream& in, HexTo& out) 
	{
		in >> std::hex >> out.value;
		return in;
	}
};

namespace detail 
{
	template <typename T>
	struct get_with_default_visitor : boost::static_visitor<T> 
	{
		T default_;
		get_with_default_visitor(T dv) : default_(dv) {}

		T const& operator()(T const& v) const 
		{ 
			return v; 
		}

		template <typename Other> T operator()(Other const&) const 
		{
			return default_;
		}
	};
};

class ParseInput
{
private:
	std::vector<boost::variant<
		float,
		unsigned long,
		long,
		unsigned long long,
		long long,
		float,
		double,
		long double,
		std::string
		>> data;
	unsigned long current_param;
	unsigned char delimeter;
	long current_param_get;
	bool DoParse(char f, std::string& input, size_t& current_pos, unsigned char delim = ' ');
	std::string format;
public:
	ParseInput(unsigned char delim = ' ');
	ParseInput(std::string _format, std::string input, unsigned char delim = ' ');
	void ExecuteManualParse(std::string input, unsigned char delim = ' ');
	void SetFormat(std::string _format, unsigned char delim = ' ');
	/*
		i,l,d - signed long
		I,L,D - unsigned long
		u - signed long long
		U - unsigned long long
		x - signed long hex
		X - unsigned long hex
		m - signed long long hex
		M - unsigned long long hex
		f - float
		F - allow only positive float
		z - double
		Z - allow only positive double
		q - long double
		Q - allow only positive long double
		w - std::string ( one word )
		s - std::string ( remaining string (format process stops after this) )

		Good to get number of succesfully parsed params
	*/
	template<typename T> T Get(size_t i = 0)
	{
		current_param_get = i;
		if (i < data.size())
		{
			return boost::apply_visitor(detail::get_with_default_visitor<T>(T()), data[i]);
		}
		return T();
	}
	/*
		i,l,d - signed long
		I,L,D - unsigned long
		u - signed long long
		U - unsigned long long
		x - signed long hex
		X - unsigned long hex
		m - signed long long hex
		M - unsigned long long hex
		f - float
		F - allow only positive float
		z - double
		Z - allow only positive double
		q - long double
		Q - allow only positive long double
		w - std::string ( one word )
		s - std::string ( remaining string (format process stops after this) )

		Good to get number of succesfully parsed params
	*/
	template<typename T> T GetNext()
	{
		if (++current_param_get < (long)data.size())
		{
			return boost::apply_visitor(detail::get_with_default_visitor<T>(T()), data[current_param_get]);
		}
		return T();
	}
	unsigned long Good();
};
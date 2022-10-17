#pragma once
#include <string>
#include <Types.h>
#include <limits>
#include <stdexcept>

// because why tf not ey ;) how did MS miss these implementations LOL
namespace std
{
	_NODISCARD inline short stos(const string& _Str, size_t* _Idx = nullptr, int _Base = 10)
	{
		constexpr short short_max = std::numeric_limits<short>::max();
		constexpr short short_min = std::numeric_limits<short>::min();

		int _Ans = stoi(_Str, _Idx, _Base);
		if (_Ans <= short_max && _Ans >= short_min)
		{
			return static_cast<short>(_Ans);
		}
		throw out_of_range(_Str);
	}

	_NODISCARD inline unsigned short stous(const string& _Str, size_t* _Idx = nullptr, int _Base = 10)
	{
		constexpr unsigned short ushort_max = std::numeric_limits<unsigned short>::max();
		constexpr unsigned short ushort_min = std::numeric_limits<unsigned short>::min();

		unsigned long _Ans = stoul(_Str, _Idx, _Base);
		if (_Ans <= ushort_max && _Ans >= ushort_min)
		{
			return static_cast<unsigned short>(_Ans);
		}
		throw out_of_range(_Str);
	}

	_NODISCARD inline char stoc(const string& _Str, size_t* _Idx = nullptr, int _Base = 10)
	{
		constexpr char char_max = std::numeric_limits<char>::max();
		constexpr char char_min = std::numeric_limits<char>::min();

		int _Ans = stoi(_Str, _Idx, _Base);
		if (_Ans <= char_max && _Ans >= char_min)
		{
			return static_cast<char>(_Ans);
		}
		throw out_of_range(_Str);
	}

	_NODISCARD inline unsigned char stouc(const string& _Str, size_t* _Idx = nullptr, int _Base = 10)
	{
		constexpr unsigned char char_max = std::numeric_limits<unsigned char>::max();
		constexpr unsigned char char_min = std::numeric_limits<unsigned char>::min();

		unsigned long _Ans = stoul(_Str, _Idx, _Base);
		if (_Ans <= char_max && _Ans >= char_min)
		{
			return static_cast<char>(_Ans);
		}
		throw out_of_range(_Str);
	}
}

namespace StrUtils
{
	void Split(const std::string& source, int index, std::string& left, std::string& right);
	bool Split(const std::string& source, char delimeter, std::string& left, std::string& right);

	bool TryCastTo(std::string& source, uint64& value, int base = 10);
	bool TryCastTo(std::string& source, uint32& value, int base = 10);
	bool TryCastTo(std::string& source, uint16& value, int base = 10);
	bool TryCastTo(std::string& source, int64& value, int base = 10);
	bool TryCastTo(std::string& source, int32& value, int base = 10);
	bool TryCastTo(std::string& source, uint8& value, int base = 10);
	bool TryCastTo(std::string& source, int16& value, int base = 10);
	bool TryCastTo(std::string& source, int8& value, int base = 10);
	bool TryCastTo(std::string& source, float& value);
	bool TryCastTo(std::string& source, double& value);

	bool TryCastFrom(std::string& destination, uint64& value, int base = 10);
	bool TryCastFrom(std::string& destination, uint32& value, int base = 10);
	bool TryCastFrom(std::string& destination, uint16& value, int base = 10);
	bool TryCastFrom(std::string& destination, uint8& value, int base = 10);
	bool TryCastFrom(std::string& destination, int64& value, int base = 10);
	bool TryCastFrom(std::string& destination, int32& value, int base = 10);
	bool TryCastFrom(std::string& destination, int16& value, int base = 10);
	bool TryCastFrom(std::string& destination, int8& value, int base = 10);
	bool TryCastFrom(std::string& destination, float& value);
	bool TryCastFrom(std::string& destination, double& value);
}
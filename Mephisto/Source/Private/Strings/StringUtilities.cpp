#include <Strings/StringUtilities.h>
#include <Core/MephistoAssert.h>
#include <stdexcept>
#include <typeinfo>
#include <spdlog/spdlog.h>

void StrUtils::Split(const std::string& source, int index, std::string& left, std::string& right)
{
	Check(index > 0);
	Check(index < source.length());
	left = std::string(source.substr(0, index));
	right = std::string(source.substr(index + 1, source.length()));
}

bool StrUtils::Split(const std::string& source, char delimiter, std::string& left, std::string& right)
{
	size_t index = source.find_first_of(delimiter, 0);
	if (index < 0) return false;
	left = std::string(source.substr(0, index));
	right = std::string(source.substr(index + 1, source.length()));
	return true;
}

bool StrUtils::TryCastTo(std::string& source, uint64& value, int base)
{
	uint64 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stoull(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(uint64).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, uint16& value, int base)
{
	uint16 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stous(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(uint16).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, uint32& value, int base)
{
	uint32 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stoul(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(uint32).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, int64& value, int base)
{
	int64 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stoll(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(int64).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, int32& value, int base)
{
	int32 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stoi(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(int32).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, uint8& value, int base)
{
	uint8 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stouc(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(uint8).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, int16& value, int base)
{
	int16 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stos(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(int16).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, int8& value, int base)
{
	int8 TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stoc(source, &LDX, base);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(int8).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, float& value)
{
	float TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stof(source, &LDX);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(float).name());
		return false;
	}
	value = TestValue;
	return true;
}

bool StrUtils::TryCastTo(std::string& source, double& value)
{
	double TestValue = 0;
	size_t LDX = 0;
	try {
		TestValue = std::stod(source, &LDX);
	}
	catch (std::invalid_argument e) {
		spdlog::error(e.what());
		return false;
	}
	catch (std::out_of_range e) {
		spdlog::error("StrUtils::TryCastTo() -> {} is out of range of type {}", e.what(), typeid(double).name());
		return false;
	}
	value = TestValue;
	return true;
}
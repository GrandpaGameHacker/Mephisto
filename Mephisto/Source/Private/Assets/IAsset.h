#pragma once
#include <string>
#include <unordered_map>
#include <memory>

union UData
{
	void* RawData;
	uint8_t* Bytes;
	std::string* Text;
	std::wstring* WideText;
};

class IAsset
{
public:
	IAsset();
	virtual ~IAsset();
	static std::shared_ptr<IAsset> LoadAsset(void* data, size_t size, std::string Name);
	virtual std::string GetType() { return "Asset"; };
	std::string GetID();
protected:
	std::string ID;
	bool bLoaded = false;
	UData Data;
	size_t Size = 0;
};
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
	static std::shared_ptr<IAsset> LoadAsset(void* data, size_t size, std::string Name, bool bCacheAsset);
	static std::shared_ptr<IAsset> GetCached(std::string name);
	static void TryAddCached(std::shared_ptr<IAsset> asset);
	virtual std::string GetType() { return "Asset"; };
	std::string GetName();
protected:
	static std::unordered_map<std::string, std::shared_ptr<IAsset>> Cache;
	std::string Name;
	bool bLoaded = false;
	bool bCached = false;
	UData Data;
	size_t Size = 0;
};
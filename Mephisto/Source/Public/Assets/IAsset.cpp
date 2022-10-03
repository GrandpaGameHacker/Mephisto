#include "Assets/IAsset.h"
#include <spdlog/spdlog.h>

std::unordered_map<std::string, std::shared_ptr<IAsset>> IAsset::Cache;

IAsset::IAsset()
{
	Name = "UNKNOWN";
	Data.RawData = nullptr;
}
IAsset::~IAsset()
{
	spdlog::info("Destroying {} -> {}", GetType(), Name);
	if (bLoaded)
	{
		delete[] Data.RawData;
		if (bCached)
		{
			Cache.erase(Name);
		}
	}
}
std::shared_ptr<IAsset> IAsset::LoadAsset(void* data, size_t size, std::string name, bool bCacheAsset)
{
	if (!Cache.empty())
	{
		auto assetPair= Cache.find(name);
		if (assetPair != Cache.end()) {
			return assetPair->second;
		}
	}
	auto asset = std::make_shared<IAsset>();
	asset->Name = name;
	asset->Size = size;
	asset->Data.RawData = new uint8_t[size];
	errno_t ErrorCopyData = memcpy_s(asset->Data.RawData, size, data, size);
	if (ErrorCopyData)
	{
		spdlog::error("IAsset::LoadAsset -> memcpy_s failure\nAsset:{} - {}", name, size);
		delete[] asset->Data.RawData;
		asset->Data.RawData = nullptr;
		asset->bLoaded = false;
		return asset;
	}
	asset->bLoaded = true;
	asset->bCached = bCacheAsset;
	if (bCacheAsset)
		Cache.try_emplace(name, asset);

	return asset;
}

std::shared_ptr<IAsset> IAsset::GetCached(std::string name)
{
	if (!Cache.empty())
	{
		auto assetPair = Cache.find(name);
		if (assetPair != Cache.end()) {
			return assetPair->second;
		}
	}
	return nullptr;
}

void IAsset::TryAddCached(std::shared_ptr<IAsset> asset)
{
	asset->bCached = true;
	Cache.try_emplace(asset->Name, asset);
}

std::string IAsset::GetName()
{
	return Name;
}

#include "Assets/IAsset.h"
#include <spdlog/spdlog.h>

IAsset::IAsset()
{
	ID = "UNKNOWN";
	Data.RawData = nullptr;
}
IAsset::~IAsset()
{
	spdlog::info("Destroying {} -> {}", GetType(), ID);
	if (bLoaded)
	{
		delete[] Data.RawData;
	}
}
std::shared_ptr<IAsset> IAsset::LoadAsset(void* data, size_t size, std::string name)
{
	auto asset = std::make_shared<IAsset>();
	asset->ID = name;
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
	return asset;
}

std::string IAsset::GetID()
{
	return ID;
}

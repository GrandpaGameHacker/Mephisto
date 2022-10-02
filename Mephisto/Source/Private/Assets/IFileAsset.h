#pragma once
#include "IAsset.h"
#include <filesystem>
#include <spdlog/spdlog.h>
enum class EFileMode
{
	Binary,
	ASCII,
	UTF16
};

class IFileAsset : public IAsset
{
public:
	IFileAsset();
	virtual ~IFileAsset();
	template <typename _Ty>
	static std::shared_ptr<_Ty> LoadAsset(std::filesystem::path filePath, bool bCacheFile);

	std::filesystem::path GetFilePath() const;
	std::string GetFileName() const;
	std::string GetFileExtension() const;
	size_t GetFileSize() const;
	void LoadInternal(std::filesystem::path filePath, bool bCacheFile);

protected:
	std::filesystem::path FilePath;
	std::string FileExt;
	size_t FileSize = NULL;

	std::string GetType() override { return "FileAsset"; };
	virtual EFileMode GetFileMode() { return EFileMode::Binary; };
};

template<typename _Ty>
inline std::shared_ptr<_Ty> IFileAsset::LoadAsset(std::filesystem::path filePath, bool bCacheFile)
{
	filePath = std::filesystem::current_path().string().append(filePath.string());
	if (!std::filesystem::exists(filePath) || !filePath.has_filename())
	{
		if (filePath.has_filename())
		{
			spdlog::error("[-] Asset Error! File {} Does not exist", filePath.filename().string());
		}
		else
		{
			spdlog::error("[-] Asset Error! File Path {} Is a directory", filePath.filename().string());
		}
		return nullptr;
	}
	std::string name = filePath.filename().string();
	auto asset = IAsset::GetCached(name);
	if (asset)
	{
		return std::dynamic_pointer_cast<_Ty>(asset);
	}
	auto fileAsset = std::make_shared<_Ty>();
	fileAsset->LoadInternal(filePath, bCacheFile);
	if (bCacheFile) {
		IAsset::TryAddCached(std::dynamic_pointer_cast<IAsset>(fileAsset));
	}
	return fileAsset;
}

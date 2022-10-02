#include "Assets/IFileAsset.h"

#include <fstream>
IFileAsset::IFileAsset()
{

}

IFileAsset::~IFileAsset()
{
}

std::filesystem::path IFileAsset::GetFilePath() const
{
	return FilePath;
}

std::string IFileAsset::GetFileName() const
{
	return Name;
}

std::string IFileAsset::GetFileExtension() const
{
	return FileExt;
}

size_t IFileAsset::GetFileSize() const
{
	return Size;
}

void IFileAsset::LoadInternal(std::filesystem::path filePath, bool bCacheFile)
{
	FilePath = filePath;
	Name = filePath.filename().string();
	FileExt = filePath.stem().string();
	bCached = bCacheFile;
	if (GetFileMode() == EFileMode::Binary)
	{
		std::ifstream FileDataStream(filePath, std::ios::binary);

		FileDataStream.seekg(0, std::ios::end);
		Size = FileDataStream.tellg();
		FileDataStream.seekg(0, std::ios::beg);
		char* Data = new char[Size];
		FileDataStream.read(Data, Size);
		delete[] Data;
	}
	else if (GetFileMode() == EFileMode::ASCII)
	{
		std::ifstream FileDataStream(filePath);
		std::string lineContent;
		std::string* Content = new std::string;
		while (std::getline(FileDataStream, lineContent))
		{
			Content->append(lineContent + "\n");
		}
		Data.Text = Content;
		bLoaded = true;
	}
}

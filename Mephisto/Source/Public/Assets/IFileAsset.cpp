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
	return FileStem;
}

std::string IFileAsset::GetFileExtension() const
{
	return FileExt;
}

size_t IFileAsset::GetFileSize() const
{
	return Size;
}

void IFileAsset::LoadInternal(std::filesystem::path filePath)
{
	ID = filePath.string();
	FilePath = filePath;
	FileStem = filePath.stem().string();
	FileExt = filePath.extension().string();
	if (GetFileMode() == EFileMode::Binary)
	{
		std::ifstream FileDataStream(filePath, std::ios::binary);

		FileDataStream.seekg(0, std::ios::end);
		Size = FileDataStream.tellg();
		FileDataStream.seekg(0, std::ios::beg);
		Data.Bytes = new uint8_t[Size];
		FileDataStream.read((char*)Data.Bytes, Size);
		bLoaded = true;
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

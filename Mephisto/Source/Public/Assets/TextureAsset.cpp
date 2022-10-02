#include "Assets/TextureAsset.h"
#include "ThirdParty/STB/stb_image.h"
#include <spdlog/spdlog.h>


TextureAsset::TextureAsset()
{
	Target = GL_TEXTURE_2D;
	Width = 0;
	Height = 0;
	BPP = 0;
	Object = 0;
	WrapS = GL_REPEAT;
	WrapT = GL_REPEAT;
	Mag = GL_LINEAR;
}

std::shared_ptr<TextureAsset> TextureAsset::LoadAsset(std::filesystem::path texturePath, GLenum Target,  bool bCacheTexture)
{
	auto textureAsset = IFileAsset::LoadAsset<TextureAsset>(texturePath, bCacheTexture);
	textureAsset->Target = Target;
	unsigned char* textureData = stbi_load_from_memory(
		textureAsset->Data.Bytes,
		textureAsset->Size,
		&textureAsset->Width,
		&textureAsset->Height,
		&textureAsset->BPP,
		NULL);

	if (!textureData) {
		spdlog::error("TextureAsset::LoadAsset() Texture file {} failed to load", textureAsset->FileName);
		return nullptr;
	}
	textureAsset->LoadOpenGLTexture(textureData);
	stbi_image_free(textureData);
	return textureAsset;
}

std::shared_ptr<TextureAsset> TextureAsset::LoadCachedAsset(std::filesystem::path texturePath, GLenum Target)
{
	static std::unordered_map<std::string, std::shared_ptr<TextureAsset>> TextureCache;
	if (!TextureCache.empty())
	{
		const auto cachedTexture = TextureCache.find(texturePath.filename().string());
		if (cachedTexture != TextureCache.end())
		{
			return cachedTexture->second;
		}
	}
	auto textureAsset = TextureAsset::LoadAsset(texturePath, Target);
	if (textureAsset) {
		TextureCache.try_emplace(textureAsset->Name, textureAsset);
	}
	return textureAsset;
}



void TextureAsset::LoadOpenGLTexture(void* textureData)
{
	glGenTextures(1, &Object);
	glBindTexture(Target, Object);

	switch (Target)
	{

	case GL_TEXTURE_3D:
		spdlog::error("TextureAsset::LoadInternal() GL_TEXTURE_3D Not Implemented");
		exit(0);
		break;

	case GL_TEXTURE_2D:
		switch (BPP)
		{
		case 1:
			glTexImage2D(Target, NULL, GL_RED, Width, Height, NULL, GL_RED, GL_UNSIGNED_BYTE, textureData);
			break;
		case 3:
			glTexImage2D(Target, NULL, GL_RGB, Width, Height, NULL, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			break;
		case 4:
			glTexImage2D(Target, NULL, GL_RGBA, Width, Height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			break;
		default:
			spdlog::error("TextureAsset::LoadInternal() Invalid BPP:{}", BPP);
			exit(0);
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		break;

	case GL_TEXTURE_1D:
		spdlog::error("TextureAsset::LoadInternal() GL_TEXTURE_1D Not Implemented");
		exit(0);
		break;
	}
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, Min);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, Mag);
	glBindTexture(Target, 0);
}

void TextureAsset::Bind(GLenum TextureUnit)
{
	glBindTextureUnit(TextureUnit, Object);
}

void TextureAsset::GetSize(int& Width, int& Height)
{
	Width = this->Width;
	Height = this->Height;
}
void TextureAsset::GetBPP(int& BPP)
{
	BPP = this->BPP;
}

void TextureAsset::SetWrapMode(GLenum S, GLenum T)
{
	glBindTexture(Target, Object);
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, S);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, T);
	glBindTexture(Target, 0);
}

void TextureAsset::SetFilterMode(GLenum MAG, GLenum MIN)
{
	glBindTexture(Target, Object);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, MIN);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, MAG);
	glBindTexture(Target, 0);
}
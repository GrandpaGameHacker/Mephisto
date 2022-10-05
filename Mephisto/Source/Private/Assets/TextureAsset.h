#pragma once
#include "Assets/IFileAsset.h"
#include <gl/glew.h>
class TextureAsset :
    public IFileAsset
{
public:
    TextureAsset();
	static std::shared_ptr<TextureAsset> LoadAsset(std::filesystem::path texturePath, GLenum Target);
	static std::shared_ptr<TextureAsset> LoadCachedAsset(std::filesystem::path texturePath, GLenum Target);
	void Bind(GLenum TextureUnit);
	void SetWrapMode(GLenum S, GLenum T);
	void SetFilterMode(GLenum MAG, GLenum MIN);
	void GetSize(int& Width, int& Height);
	void GetBPP(int& BPP);

protected:
	EFileMode GetFileMode() override { return EFileMode::Binary;}
	void LoadOpenGLTexture(void* textureData);
	GLenum Target;
	GLuint Object;
	GLenum WrapS, WrapT, Mag, Min;
	int Width;
	int Height;
	int BPP;
};

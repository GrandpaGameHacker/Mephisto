#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "Assets/IFileAsset.h"
#include <string>
#include <map>

enum class EShaderStage
{
    Vertex,
    TessCtrl,
    TessEval,
    Geometry,
    Fragment,
    Compute,
    Invalid,
};

class ShaderAsset :
    public IFileAsset
{
public:
    friend class Shader;
    ShaderAsset();
    ShaderAsset(std::string shaderFile, bool bCacheShader = false);
    bool Compile();
    static std::shared_ptr<ShaderAsset> LoadAsset(std::filesystem::path shaderPath, bool bCacheShader = false);
protected:
    bool bIsCompiled;
    GLuint ShaderObject;
    EShaderStage Stage;
    EFileMode GetFileMode() override { return EFileMode::ASCII;}
};

class Shader
{
public:
    Shader();
    ~Shader();
    void AddStageFile(EShaderStage stage, std::string File);
    bool Compile();
    void Use();
    void UseCache(bool bUseCaching);
	void Set(std::string name, bool value);
	void Set(std::string name, int value);
	void Set(std::string name, unsigned int value);
	void Set(std::string name, float value);
	void Set(std::string name, glm::vec2& value);
	void Set(std::string name, float x, float y);
	void Set(std::string name, glm::vec3& value);
	void Set(std::string name, float x, float y, float z);
	void Set(std::string name, glm::vec4& value);
	void Set(std::string name, float x, float y, float z, float w);
	void Set(std::string name, glm::mat2& value);
	void Set(std::string name, glm::mat3& value);
	void Set(std::string name, glm::mat4& value);


protected:
    void FreeStageObjects();
    std::map<EShaderStage,std::shared_ptr<ShaderAsset>> Stages;
    bool bIsCompiled;
    bool bUseCaching;
    GLuint ShaderProgram;
};
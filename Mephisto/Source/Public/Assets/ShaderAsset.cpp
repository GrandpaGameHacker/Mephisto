#include "Assets/ShaderAsset.h"
#include <spdlog/spdlog.h>

ShaderAsset::ShaderAsset() :
	ShaderObject(NULL), bIsCompiled(false), Stage(EShaderStage::Invalid)
{

}

ShaderAsset::ShaderAsset(std::string shaderFile, bool bCacheShader) :
	ShaderObject(NULL), bIsCompiled(false), Stage(EShaderStage::Invalid)
{
	
}

bool ShaderAsset::Compile()
{
	if (!bLoaded) {
		spdlog::error("Shader load failure: {}", Name);
	}
	auto shaderCode = Data.Text->c_str();
	switch (Stage)
	{
	case EShaderStage::Vertex:
		ShaderObject = glCreateShader(GL_VERTEX_SHADER);
		break;
	case EShaderStage::Compute:
		ShaderObject = glCreateShader(GL_COMPUTE_SHADER);
		break;
	case EShaderStage::Fragment:
		ShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case EShaderStage::Geometry:
		ShaderObject = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case EShaderStage::TessCtrl:
		ShaderObject = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;
	case EShaderStage::TessEval:
		ShaderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;
	default:
		break;
	}
	glShaderSource(ShaderObject, 1, &shaderCode, nullptr);
	glCompileShader(ShaderObject);

	GLint isVertexCompiled = 0;
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &isVertexCompiled);
	if (isVertexCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(ShaderObject, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(ShaderObject, maxLength, &maxLength, infoLog.data());
		glDeleteShader(ShaderObject);
		std::string logInfo = infoLog.data();
		spdlog::error("Shader::Compile() VertexError: {}", logInfo);
		return false;
	}
	bIsCompiled = true;
	return true;
}

std::shared_ptr<ShaderAsset> ShaderAsset::LoadAsset(std::filesystem::path shaderPath, bool bCacheShader)
{
	return IFileAsset::LoadAsset<ShaderAsset>(shaderPath, bCacheShader);
}

Shader::Shader() : 
	ShaderProgram(NULL), bIsCompiled(false)
{

}

Shader::~Shader()
{

}

void Shader::AddStageFile(EShaderStage stage, std::string File)
{
	if (Stages.find(stage) == Stages.end())
	{
		auto shader = ShaderAsset::LoadAsset(File, bUseCaching);
		
		shader->Stage = stage;
		Stages.try_emplace(stage, shader);
	}
}

bool Shader::Compile()
{
	ShaderProgram = glCreateProgram();
	for (auto stage : Stages)
	{
		if (stage.second->Compile())
		{
			glAttachShader(ShaderProgram, stage.second->ShaderObject);
		}
	}
	glLinkProgram(ShaderProgram);
	GLint isLinked = 0;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(ShaderProgram, maxLength, &maxLength, infoLog.data());
		glDeleteProgram(ShaderProgram);
		FreeStageObjects();
		std::string logInfo = infoLog.data();
		spdlog::error("Shader::Compile() Link Error: {}", logInfo);
		return false;
	}
	bIsCompiled = true;
	return true;
}

void Shader::FreeStageObjects()
{
	for (auto stage : Stages)
	{
		glDeleteShader(stage.second->ShaderObject);
	}
}

void Shader::Bind()
{
	if (bIsCompiled)
	{
		glUseProgram(ShaderProgram);
	}
}

void Shader::UseCache(bool bUseCaching)
{
	this->bUseCaching = bUseCaching;
}

void Shader::Set(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, unsigned int value)
{
	glUniform1i(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(ShaderProgram, name.c_str()), value);
}

void Shader::Set(std::string name, glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y);
}

void Shader::Set(std::string name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z);
}

void Shader::Set(std::string name, glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, &value[0]);
}

void Shader::Set(std::string name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ShaderProgram, name.c_str()), x, y, z, w);
}

void Shader::Set(std::string name, glm::mat2& value)
{
	glUniformMatrix2fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::Set(std::string name, glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::Set(std::string name, glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &value[0][0]);
}


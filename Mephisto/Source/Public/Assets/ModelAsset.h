#pragma once
#include "Assets/ShaderAsset.h"
#include "Assets/TextureAsset.h"
#include <vector>
#include <glm/glm.hpp>
#include <mutex>
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/PostProcess.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


typedef std::vector<Vertex> VertexList;
typedef std::vector<unsigned int> IndexList;
typedef std::vector<std::shared_ptr<TextureAsset>> TextureList;


class Mesh
{
public:
	friend class ModelAsset;
	Mesh();
	Mesh(VertexList vertices, IndexList indices, TextureList textures);
	void Draw(Shader& shader);
private:
	void SetupMesh();
	VertexList Vertices;
	IndexList Indices;
	TextureList Textures;
	GLuint VAO, VBO, EBO;
};

class ModelAsset : public
	IFileAsset
{
public:
	ModelAsset();
	~ModelAsset();
	static std::shared_ptr<ModelAsset> LoadAsset(std::filesystem::path modelPath);
	static std::shared_ptr<ModelAsset> LoadCachedAsset(std::filesystem::path modelPath);
	void Draw(Shader& shader);
	std::string GetType() override { return "ModelAsset"; };
	EFileMode GetFileMode() override;
private:
	void ProcessNode(aiNode* Node, const aiScene* scene);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* Mesh, const aiScene* scene);
	std::vector<std::shared_ptr<TextureAsset>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name);
	std::vector<std::shared_ptr<Mesh>> Meshes;
	std::string Path;
};


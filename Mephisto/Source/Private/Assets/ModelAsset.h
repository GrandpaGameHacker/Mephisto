#pragma once
#include "Assets/IFileAsset.h"
#include "Assets/ShaderAsset.h"
#include "Assets/TextureAsset.h"
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


typedef std::vector<Vertex> VertexList;
typedef std::vector<unsigned int> IndexList;
typedef std::vector<void*> TextureList;


class Mesh
{
public:
	friend class ModelAsset;
	Mesh();
	Mesh(VertexList vertices, IndexList indices, TextureList textures);
	void Draw(Shader& shader);
};

class ModelAsset
{

};


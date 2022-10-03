#include "Assets/ModelAsset.h"

Mesh::Mesh() : 
	Vertices(),
	Indices(),
	Textures(),
	VAO(0), VBO(0), EBO(0)
{
}

Mesh::Mesh(VertexList vertices, IndexList indices, TextureList textures)
	:
	Vertices(vertices),
	Indices(indices),
	Textures(textures),
	VAO(0), VBO(0), EBO(0)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNumber = 1;
	unsigned int specularNumber = 1;
	for (unsigned int i = 0; i < Textures.size(); i++) {
		std::string number;
		std::string name = Textures[i]->GetFileName();
		if (name == "diffuse") {
			number = std::to_string(diffuseNumber++);
		}
		else if (name == "specular") {
			number = std::to_string(specularNumber++);
		}
		else {
			spdlog::error("Mesh::Draw() {}", "Texture not named specular/diffuse");
		}
		shader.Set("material." + name + number, i);
		Textures[i]->Bind(i);
	}
	glBindVertexArray(VAO);
	GLint size = GLint(Indices.size());
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

ModelAsset::ModelAsset()
{

}

ModelAsset::~ModelAsset()
{
}

std::shared_ptr<ModelAsset> ModelAsset::LoadAsset(std::filesystem::path modelPath)
{
	auto asset = IFileAsset::LoadAsset<ModelAsset>(modelPath);
	Assimp::Importer import;
	auto current_path = std::filesystem::current_path();
	current_path.concat(modelPath.string());
	const aiScene* scene = import.ReadFile(current_path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		spdlog::error("Model::LoadModel()", "SCENE LOAD FAIL");
		return nullptr;
	}
	asset->Path = modelPath.parent_path().string();
	asset->ProcessNode(scene->mRootNode, scene);
	return asset;
}

std::shared_ptr<ModelAsset> ModelAsset::LoadCachedAsset(std::filesystem::path modelPath)
{
	static std::mutex mut;
	static std::unordered_map <std::string, std::shared_ptr<ModelAsset>> ModelCache;
	std::scoped_lock lock(mut);

	if (!ModelCache.empty())
	{
		const auto cachedModel = ModelCache.find(modelPath.string());
		if (cachedModel != ModelCache.end())
		{
			return cachedModel->second;
		}
	}

	auto asset = ModelAsset::LoadAsset(modelPath);
	if (asset)
	{
		ModelCache.try_emplace(modelPath.string(), asset);
	}
	return asset;
}


void ModelAsset::Draw(Shader& shader)
{
	for (auto& Mesh : Meshes) {
		Mesh->Draw(shader);
	}
}

EFileMode ModelAsset::GetFileMode()
{
	if (FileExt == ".obj") {
		return EFileMode::ASCII;
	}
	return EFileMode::Binary;
}

void ModelAsset::ProcessNode(aiNode* Node, const aiScene* scene)
{
	for (unsigned int i = 0; i < Node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[Node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		ProcessNode(Node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh> ModelAsset::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	VertexList vertices;
	IndexList indices;
	TextureList textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		TextureList diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		TextureList specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return std::make_shared<Mesh>(vertices, indices, textures);
}

std::vector<std::shared_ptr<TextureAsset>> ModelAsset::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name)
{
	TextureList textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		auto texture = TextureAsset::LoadCachedAsset(Path + "/" + str.C_Str(), GL_TEXTURE_2D);
		textures.push_back(texture);
	}
	return textures;
}

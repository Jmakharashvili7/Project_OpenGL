#include "Model.h"

#include "Shader.h"

Model::Model(char* path)
{
	LoadModel(path);
}

void Model::LoadModel(std::string path)
{
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
}

Mesh Model::ProcessMesh()
{
	return Mesh();
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string TypeName)
{
	return std::vector<Texture>();
}

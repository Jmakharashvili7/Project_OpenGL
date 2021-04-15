#pragma once
#ifndef MODEL_H 
#define MODEL_H

// assimp library
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "structures.h"
#include "Mesh.h"

class Shader;

class Model
{
public:
	Model(char* path);
	void Draw(Shader &shader);
private:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh();
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string TypeName);
};
#endif // MODEL_H


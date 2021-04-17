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

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	// model data 
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded; 
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;
public:
	Model(char* path, bool gamma = false);
	void Draw(Shader &shader);
private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string TypeName);
};
#endif // MODEL_H


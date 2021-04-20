#include "Model.h"

#include <stb_image.h>
#include <iostream>

#include "Mesh.h"


Model::Model(std::string path, bool gamma) 
	: gammaCorrection(gamma)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	// draw all the individual meshes
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	/* read the file at the specified path
	   aiProcess_Triangulate makes sure the whole mode is only made out of triangles
	   aiProcess_FlipUVs flips the y cordinate to match openGL which has descenting Y cordinate from top left corner*/
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check that the scene object was loaded correctly if not print the error
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string error = import.GetErrorString();
		LOG("ERROR::ASSIMP::" + error);
		return;
	}
	// save the path of the model 
	directory = path.substr(0, path.find_last_of('/'));

	/* a recursive function to process all the nodes and its children as each node might have a child node
	   exit condition is if all nodes have been processed */
	ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// keep repeating until no nodes are left
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{

	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
    std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// temp variable to hold the vertex data
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector due to type conversion problems

		// retrieve the position vertices
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// check if mesh has any normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;

			/* retrieve the texture coords
			   A model can have up to 8 texture coords but we only care about the first one
			   some models might not have texture coords so we need to check for it */
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// check if mesh has any material
	if (mesh->mMaterialIndex >= 0)
	{
		// retrieve the material by accessing the scene material list by using the material index from the mesh
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		/* we assume a convention for sampler names in the shaders. Each texture should be named
		   as 'texture_typeN' where N is a number ranging from 1 to MAX_SAMPLER_NUMBER and type is type of texture
		   Below are texture types we accept:
		   diffuse: texture_diffuseN
		   specular: texture_specularN
		   normal: texture_normalN
		   height: */

		   // 1. diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}
	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		// loop through all the textures loaded
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			// check if the current file path and the loaded texture file path is the same
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // if it is the same we skip loading the current texture
				break;
			}
		}

		// if texture hasn't been loaded already, load it
		if (!skip)
		{   
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	/* the path of the models texture and the model is usually the same so we can combine the
	   directory path and the texture path */
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	GLCall(glGenTextures(1, &textureID));

	// load the texture using stb_image
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	// check if the data was loaded 
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		/* GL_TEXTURE_WRAP_S is the horizontal wrap that determines how the texture will be wraped horizontally
	       GL_REPEAT means the texture will be repeated to wrap over the model */
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

		/* GL_TEXTURE_WRAP_S is the vertical wrap that determines how the texture will be wraped vertically
		   GL_REPEAT means the texture will be repeated to wrap over the model */
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		/* GL_TEXTURE_MIN_FILTER is how our texture will be scaled down in case in needs to be rendererd smaller
		   GL_LINER_MIPMAP_LINEAR Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the 
		   GL_LINEAR criterion (a weighted average of the texture elements that are closest to the specified texture coordinates) 
		   to produce a texture value from each mipmap. The final texture value is a weighted average of those two values */
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

		/* GL_TEXTURE_MAG_FILTER is how our texture will be scaled up in case it needs to be rendered bigger
	   GL_LINER insures it will not snap to the nearest pixel and will look linear*/
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(data);
	}
	else
	{
		std::string error = path;
		LOG("Texture failed to load at path: " + error);
		stbi_image_free(data);
	}

	return textureID;
}

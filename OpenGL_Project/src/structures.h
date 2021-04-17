#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

#include <string>
#include "GL/glew.h"
#include "Renderer.h"

#include <glm/vec2.hpp> // vec2, bvec2, dvec2, ivec2 and uvec2
#include <glm/vec3.hpp> // vec3, bvec3, dvec3, ivec3 and uvec3
#include <glm/vec4.hpp> // vec4, bvec4, dvec4, ivec4 and uvec4
#include <glm/mat2x2.hpp> // mat2, dmat2
#include <glm/mat2x3.hpp> // mat2x3, dmat2x3
#include <glm/mat2x4.hpp> // mat2x4, dmat2x4
#include <glm/mat3x2.hpp> // mat3x2, dmat3x2
#include <glm/mat3x3.hpp> // mat3, dmat3
#include <glm/mat3x4.hpp> // mat3x4, dmat2
#include <glm/mat4x2.hpp> // mat4x2, dmat4x2
#include <glm/mat4x3.hpp> // mat4x3, dmat4x3
#include <glm/mat4x4.hpp> // mat4, dmat4
#include <glm/common.hpp> // all the GLSL common functions
#include <glm/exponential.hpp> // all the GLSL exponential functions
#include <glm/geometric.hpp> // all the GLSL geometry functions
#include <glm/integer.hpp> // all the GLSL integer functions
#include <glm/matrix.hpp> // all the GLSL matrix functions
#include <glm/packing.hpp> // all the GLSL packing functions
#include <glm/trigonometric.hpp> // all the GLSL trigonometric functions
#include <glm/vector_relational.hpp> // all the GLSL vector relational functions

struct Vertex {
	glm::vec3 position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path; // store the path of the texture to compare with other textures
};

struct Vector3 
{ 
	float x, y, z;

	Vector3()
	{
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Vector4
{
	float x, y, z, w;

	Vector4()
	{
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};

struct Lighting
{
	Vector4 ambient, diffuse, specular;

	Lighting()
	{
	}

	Lighting(Vector4 ambient, Vector4 diffuse, Vector4 specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
};

struct Material
{
	Vector4 ambient, diffuse, specular;
	GLfloat shininess;

	Material() {  }

	Material(Vector4 ambient, Vector4 diffuse, Vector4 specular, GLfloat shininess)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}
};

struct Color
{
	GLfloat r, g, b;
};

struct TexCoord
{
	GLfloat u, v;
};

struct VertexBufferElement
{
private:

public:
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:		   return 4;
		case GL_UNSIGNED_INT:  return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};
	
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentShader;
};

#endif // STRUCTURES_H
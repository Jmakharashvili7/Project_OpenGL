#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

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

struct Camera 
{ 
	Vector3 eye, center, up;
};

struct Color
{
	GLfloat r, g, b;
};

struct Vertex
{
	GLfloat x, y, z;
};

struct TexCoord
{
	GLfloat u, v;
};

struct Mesh
{
	Vertex* vertices;
	Vector3* normals;
	GLushort* indices;
	TexCoord* texCoords;
	int vertexCount, normalCount, indexCount, texCoordCount;	

	Mesh()
	{
		vertices = nullptr;
		normals = nullptr;
		indices = nullptr;

		vertexCount = 0;
		normalCount = 0;
		indexCount = 0;
	}
};
#endif // STRUCTURES_H
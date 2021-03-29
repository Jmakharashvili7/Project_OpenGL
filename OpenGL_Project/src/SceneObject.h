#pragma once
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "structures.h"

class Texture2D;

class SceneObject
{
protected:
	Mesh* m_mesh;
	Texture2D* m_texture;
public:
	SceneObject(Mesh* mesh, Texture2D* texture);
	virtual ~SceneObject();

	virtual void Update();
	virtual void Draw();
};
#endif // SCENEOBJECT_H

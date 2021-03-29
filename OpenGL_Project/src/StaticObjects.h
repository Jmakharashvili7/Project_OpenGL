#pragma once
#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "SceneObject.h"

class StaticObject : public SceneObject
{
private:
	Vector3 m_position;
public:
	StaticObject(Mesh* mesh, Texture2D* texture, float x, float y, float z);
	~StaticObject();

	void Draw();
	void Update();
};
#endif // STATICOBJECTS_H


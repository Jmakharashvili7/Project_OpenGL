#include "SceneObject.h"
#include "Texture2D.h"

SceneObject::SceneObject(Mesh* mesh, Texture2D* texture)
{
	m_mesh = mesh;
	m_texture = texture;
}

SceneObject::~SceneObject()
{
}

void SceneObject::Update()
{
}

void SceneObject::Draw()
{
}

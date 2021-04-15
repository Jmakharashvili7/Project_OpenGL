#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include "Renderer.h"

class Texture2D
{
private:
	unsigned int m_rendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP; // bits per pixel
public:
	Texture2D(const std::string& path);
	~Texture2D();

	void Bind(unsigned int slot = 0) const; // option to specify slot (default 0 if not specified)
	void Unbind();

	inline int GetHeight() const { return m_height; }
	inline int GetWidth() const { return m_width; }
};
#endif // TEXTURE_H

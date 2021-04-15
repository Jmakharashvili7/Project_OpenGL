#include "Texture.h"
#include "stb_image.h"

Texture2D::Texture2D(const std::string& path)
	: m_rendererID(0), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	/* flip the texture as stb_image loads the image from top right corner and openGL expects
	   to start from bottom left. Flipping the image will set bottom left of the image to (0,0) */
	stbi_set_flip_vertically_on_load(1); 

	/* pass in the variables for the texture so that the function can write to it and let it know
	   the dimensions of the image. The 4 stands for desired channels, as we want RGBA we pass in 4*/
	m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID)); 

	/* GL_TEXTURE_MIN_FILTER is how our texture will be scaled down in case it needs to be rendered smaller
	   GL_LINEAR insures it will not snap to the nearest pixel and will look linear*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	/* GL_TEXTURE_MAG_FILTER is how our texture will be scaled up in case it needs to be rendered bigger
	   GL_LINER insures it will not snap to the nearest pixel and will look linear*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	/* GL_TEXTURE_WRAP_S is the horizontal wrap that determines how the texture will be wraped horizontally
	   GL_CLAMP means the area will not be extended*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

	/* GL_TEXTURE_WRAP_T is the vertical wrap that determines how the texture will be wraped horizontally
	   GL_CLAMP means the area will not be extended*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/* 0 is for level-of-detail which in our case is none. 
	   GL_RGBA8 is the internal format. In our case its RGBA and its 8 bits
	   second zero is the border which we dont need
	   GL_RGBA is the format of the pixel data which is also RGBA
	   GL_UNSIGNED_BYTE is the type of data we are passing in*/
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

	// unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// if the local buffer isnt empty then free it
	if (m_localBuffer)
	{
		stbi_image_free(m_localBuffer);
	}
}

Texture2D::~Texture2D()
{
	GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture2D::Bind(unsigned int slot /* = 0 */) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // assign the texture to a passed in slot
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture2D::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

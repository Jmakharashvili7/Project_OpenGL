#include <fstream>
#include "Texture2D.h"

using namespace std;

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}

bool Texture2D::Load(std::string path, int width, int height)
{
	char* tempTextureData; int fileSize; ifstream inFile;
	m_width = width; m_height = height;
	inFile.open(path, ios::binary);

	if (!inFile.good())
	{
		cerr << "Can't open texture file " << path << endl;
		return false;
	}

	inFile.seekg(0, ios::end); // seek to the end of the file
	fileSize = (int)inFile.tellg(); // get current position in the file - The end, this gives us total file size
	tempTextureData = new char[fileSize]; // create new array to store data
	inFile.seekg(0, ios::beg); // seek back to begging of the file
	inFile.read(tempTextureData, fileSize); // read all the data in one go
	inFile.close();

	cout << path << " loaded." << endl;
		
	// Generate texture ID
	glGenTextures(1, &m_ID); 
	// bind the texture to the ID
	glBindTexture(GL_TEXTURE_2D, m_ID); 

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);

	// Use GL_LINEAR for textured objects and GL_NEAREST for untextured objects
	// using nearest filtering will result in a whitewashed object (your texture will not show).
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete[] tempTextureData; // clear up the data
	return true;
}

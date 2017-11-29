#pragma once

#include <GL\glew.h>

#include "TextureData.h"

class Texture
{
private:
	const GLuint m_id;
public:
	//TODO: temporary
	GLuint m_normalMap = -1;
public:
	const int numberOfRows;
	float reflectivity = 0;
	float shineDamper = 1;
	bool hasTransparency = false;
	bool useFakeLighting = false;
public:
	Texture(GLuint id);

	// Binds the texture
	void bind() const;

	// Unbinds the texture
	void unbind() const;

	// Returns the OpenGL ID of the texture
	GLuint getID() const;
};
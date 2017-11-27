#pragma once

#include <GL\glew.h>

#include "TextureData.h"

class Texture
{
private:
	GLuint m_id;
public:
	const int numberOfRows = 1;
	float reflectivity = 0;
	float shineDamper = 1;
	bool hasTransparency = false;
	bool useFakeLighting = false;
public:
	Texture(const GLuint id);

	// Binds the texture
	void bind() const;

	// Unbinds the texture
	void unbind() const;

	//TODO: Remove
	GLuint getID() const
	{
		return m_id;
	}
};
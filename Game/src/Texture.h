#pragma once

#include "Loader.h"

class Texture
{
private:
	unsigned int m_id;
public:
	float shineDamper = 1;
	float reflectivity = 0;
	bool hasTransparency = false;
	bool useFakeLighting = false;
public:
	Texture(const std::string& filePath)
	{
		m_id = Loader::loadTexture(filePath).id;
	}

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int getID() const
	{
		return m_id;
	}
};
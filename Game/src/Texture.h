#pragma once

class Texture
{
private:
	unsigned int m_texture;
public:
	Texture(const std::string& filePath)
	{
		glGenTextures(1, &m_texture);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Couldn't load texture" << std::endl;
		}
	}
};
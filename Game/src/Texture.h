#pragma once

class Texture
{
private:
	unsigned int m_id;
public:
	float shineDamper = 1;
	float reflectivity = 0;
	bool hasTransparency = false;
public:
	Texture(const std::string& filePath)
	{
		glGenTextures(1, &m_id);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
		std::cout << nrChannels << std::endl;
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			glTexImage2D(
				GL_TEXTURE_2D, // Target/type of texture
				0, // Level, always 0
				GL_RGBA, // Format to store the texture in, always store with Alpha channel
				width, // Width of texture
				height, // Height of texture
				0, // Border, always 0
				(nrChannels == 3 ? GL_RGB : GL_RGBA), // Channels of input texture
				GL_UNSIGNED_BYTE, // Data type of input texture
				data // Texture data
			);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			std::cout << "Couldn't load texture" << std::endl;
		}
		if (nrChannels == 4)
		{
			hasTransparency = true;
		}
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
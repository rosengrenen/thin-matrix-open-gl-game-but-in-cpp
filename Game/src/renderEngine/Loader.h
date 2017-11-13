#ifndef LOADER_H
#define LOADER_H

#include <GL\glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb_image.h"

#include "..\models\RawModel.h"

class Loader
{
private:
	std::vector<int> m_vaos;
	std::vector<int> m_vbos;
	std::vector<int> m_textures;
private:
	unsigned int createVAO()
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		m_vaos.push_back(vao);
		glBindVertexArray(vao);
		return vao;
	}

	void storeDataInAttribList(const int attribNumber, const int coordSize, const std::vector<float> data)
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		m_vbos.push_back(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribNumber, coordSize, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void unbindVAO()
	{
		glBindVertexArray(0);
	}

	void bindIndicesBuffer(std::vector<int> indices)
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		m_vbos.push_back(vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
	}
public:
	RawModel loadToVAO(std::vector<float> positions, std::vector<float> texCoords, std::vector<int> indices)
	{
		unsigned int vao = createVAO();
		bindIndicesBuffer(indices);
		storeDataInAttribList(0, 3, positions);
		storeDataInAttribList(1, 2, texCoords);
		unbindVAO();
		return RawModel(vao, indices.size());
	}

	unsigned int loadTexture(const std::string& fileName)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		m_textures.push_back(texture);
		int width, height, nrChannels;
		unsigned char *data = stbi_load(std::string("res/textures/" + fileName).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture: res/textures/" << fileName << std::endl;
		}
		stbi_image_free(data);
		return texture;
	}
	~Loader()
	{
		for (auto vao : m_vaos)
			glDeleteVertexArrays(1, (const GLuint*) &vao);

		for (auto vbo : m_vbos)
			glDeleteBuffers(1, (const GLuint*) &vbo);

		for (auto texture : m_textures)
			glDeleteTextures(1, (const GLuint*) &texture);
	}
};

#endif
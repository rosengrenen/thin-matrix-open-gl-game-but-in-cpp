#ifndef LOADER_H
#define LOADER_H

#include <GL\glew.h>

#include "RawModel.h"

class Loader
{
private:
	std::vector<int> m_vaos;
	std::vector<int> m_vbos;
private:
	unsigned int createVAO()
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		m_vaos.push_back(vao);
		glBindVertexArray(vao);
		return vao;
	}

	void storeDataInAttribList(const int attribNumber, const std::vector<float> data)
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		m_vbos.push_back(vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribNumber, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
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
	RawModel loadToVAO(std::vector<float> positions, std::vector<int> indices)
	{
		unsigned int vao = createVAO();
		bindIndicesBuffer(indices);
		storeDataInAttribList(0, positions);
		unbindVAO();
		return RawModel(vao, indices.size());
	}
	unsigned int loadTexture(std::string& fileName)
	{
		return 0;
	}
	~Loader()
	{
		for (auto vao : m_vaos)
			glDeleteVertexArrays(1, (const GLuint*) &vao);

		for (auto vbo : m_vbos)
			glDeleteBuffers(1, (const GLuint*) &vbo);
	}
};

#endif
#pragma once

class Model
{
private:
	unsigned int m_vao;
public:
	int numVertices;
public:
	Model(unsigned int vao, int numVertices) : m_vao(vao), numVertices(numVertices)
	{ }

	void bind()
	{
		glBindVertexArray(m_vao);
	}

	void unbind()
	{
		glBindVertexArray(0);
	}
};
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

	void bind() const
	{
		glBindVertexArray(m_vao);
	}

	void unbind() const
	{
		glBindVertexArray(0);
	}

	unsigned int getID() const
	{
		return m_vao;
	}
};
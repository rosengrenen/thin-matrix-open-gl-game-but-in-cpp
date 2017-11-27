#pragma once

#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class RawModel
{
private:
	GLuint m_vao;
	int m_vertexCount;
public:
public:
	//Model(unsigned int vao, int numVertices);

	// Create a vertex array object from vertex positions, texture coordinates, normals and indices
	RawModel(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<int>& indices);

	// Create a simple vertex array object from vertex positions
	RawModel(const std::vector<float>& positions, GLint dimensions);

	void bind() const;

	void unbind() const;

	const int getVertexCount() const;

	//TODO: Remove
	GLuint getID() const
	{
		return m_vao;
	}
private:
	void bindIndicesBuffer(const std::vector<int>& indices);

	void storeDataInAttribList(int attributePosition, int size, const std::vector<float>& data);
};
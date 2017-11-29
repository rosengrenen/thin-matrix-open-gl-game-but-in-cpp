#pragma once

#include <glm\vec3.hpp>
#include <glm\glm.hpp>

#include <vector>

class VertexNM
{
public:
	const int NO_INDEX = -1;

	glm::vec3 m_position;
	int m_textureIndex = NO_INDEX;
	int m_normalIndex = NO_INDEX;
	VertexNM* m_duplicateVertex;
	int m_index;
	float m_length;
	std::vector<glm::vec3> m_tangents;
	glm::vec3 m_averagedTangent;
public:
	VertexNM(int index, const glm::vec3& position) : m_index(index), m_position(position), m_length(glm::length(position))
	{ }

	void addTangent(const glm::vec3& tangent)
	{
		m_tangents.push_back(tangent);
	}

	//NEW
	VertexNM duplicate(int newIndex)
	{
		VertexNM vertex(newIndex, m_position);
		vertex.m_tangents = m_tangents;
		return vertex;
	}

	void averageTangents()
	{
		if (m_tangents.empty())
		{
			return;
		}
		for (auto& tangent : m_tangents)
		{
			m_averagedTangent += tangent;
		}
		m_averagedTangent = glm::normalize(m_averagedTangent);
	}

	bool isSet()
	{
		return m_textureIndex != NO_INDEX && m_normalIndex != NO_INDEX;
	}

	bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)
	{
		return textureIndexOther == m_textureIndex && normalIndexOther == m_normalIndex;
	}

	VertexNM* getDuplicateVertex()
	{
		return m_duplicateVertex;
	}

	void setDuplicateVertex(VertexNM* duplicateVertex)
	{
		delete m_duplicateVertex;
		m_duplicateVertex = duplicateVertex;
	}
};
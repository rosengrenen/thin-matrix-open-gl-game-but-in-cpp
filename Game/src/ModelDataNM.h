#pragma once

#include <vector>

class ModelDataNM
{
public:
	std::vector<float> m_vertices;
	std::vector<float> m_texCoords;
	std::vector<float> m_normals;
	std::vector<float> m_tangents;
	std::vector<int> m_indices;
	float m_furthestPoint;
public:
	ModelDataNM(const std::vector<float>& vertices,
		const std::vector<float>& texCoords,
		const std::vector<float>& normals,
		const std::vector<float>& tangents,
		const std::vector<int>& indices,
		float furthestPoint)
		:
		m_vertices(vertices),
		m_texCoords(texCoords),
		m_normals(normals),
		m_tangents(tangents),
		m_indices(indices),
		m_furthestPoint(furthestPoint)
	{ }
};
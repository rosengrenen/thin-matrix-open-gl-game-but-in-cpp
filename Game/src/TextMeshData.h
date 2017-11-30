#pragma once

#include <vector>

class TextMeshData
{
private:
	std::vector<float> m_vertexPositions;
	std::vector<float> m_textureCoords;
public:
	TextMeshData(const std::vector<float>& vertexPositions, const std::vector<float>& textureCoords) :
		m_vertexPositions { vertexPositions },
		m_textureCoords { textureCoords }
	{ }

	std::vector<float> getVertexPositions()
	{
		return m_vertexPositions;
	}

	std::vector<float> getTextureCoords()
	{
		return m_textureCoords;
	}

	int getVertexCount()
	{
		return m_vertexPositions.size() / 2;
	}
};
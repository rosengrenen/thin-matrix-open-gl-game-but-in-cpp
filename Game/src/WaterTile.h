#pragma once

class WaterTile
{
public:
	const float TILE_SIZE = 400;
private:
	float m_x, m_z;
	float m_height;
public:
	WaterTile(float centerX, float centerZ, float height) : m_x(centerX), m_z(centerZ), m_height(height)
	{}

	float getHeight()
	{
		return m_height;
	}

	float getX()
	{
		return m_x;
	}

	float getZ()
	{
		return m_z;
	}

	glm::mat4 getModelMatrix() const
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_height, m_z));
		return glm::scale(model, glm::vec3(TILE_SIZE));
	}
};
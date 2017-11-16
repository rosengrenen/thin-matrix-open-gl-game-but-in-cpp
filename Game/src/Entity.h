#pragma once

#include <glm\gtc\matrix_transform.hpp>

class Entity
{
private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	float m_scale;
public:
	Entity(glm::vec3 position, glm::vec3 rotation, float scale) : m_position(position), m_rotation(rotation), m_scale(scale)
	{ }

	void move(float dx, float dy, float dz)
	{
		m_position.x += dx;
		m_position.y += dy;
		m_position.z += dz;
	}

	void rotate(float dx, float dy, float dz)
	{
		m_rotation.x += dx;
		m_rotation.y += dy;
		m_rotation.z += dz;
	}

	glm::mat4 getTransformationMatrix()
	{
		// Translate
		glm::mat4 transformation = glm::translate(glm::mat4(1), m_position);
		// RotateX
		transformation = glm::rotate(transformation, m_rotation.x, glm::vec3(1, 0, 0));
		// RotateY
		transformation = glm::rotate(transformation, m_rotation.y, glm::vec3(0, 1, 0));
		// RotateZ
		transformation = glm::rotate(transformation, m_rotation.z, glm::vec3(0, 0, 1));
		// Scale
		return glm::scale(transformation, glm::vec3(m_scale));
	}
};
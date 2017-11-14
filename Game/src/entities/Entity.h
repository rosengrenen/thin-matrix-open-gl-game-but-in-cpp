#ifndef ENTITY_H
#define ENTITY_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "..\models\TexturedModel.h"

class Entity
{
private:
	TexturedModel m_model;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	float m_scale;
public:
	Entity(const TexturedModel model, const glm::vec3 position, const glm::vec3 rotation, const float scale) : m_model(model), m_position(position), m_rotation(rotation), m_scale(scale) { }

	glm::mat4 getTransformationMatrix()
	{
		// Translate
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), m_position);
		// Rotate X
		matrix = glm::rotate(matrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		// Rotate Y
		matrix = glm::rotate(matrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		// Rotate Z
		matrix = glm::rotate(matrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		// Scale
		matrix = glm::scale(matrix, glm::vec3(m_scale, m_scale, m_scale));

		return matrix;
	}

	void move(glm::vec3 positionChange)
	{
		m_position += positionChange;
	}

	void rotate(glm::vec3 rotationChange)
	{
		m_rotation += rotationChange;
	}

	TexturedModel getModel() const
	{
		return m_model;
	}

	glm::vec3 getPosition() const
	{
		return m_position;
	}

	glm::vec3 getRotation() const
	{
		return m_rotation;
	}

	float getScale() const
	{
		return m_scale;
	}
};

#endif
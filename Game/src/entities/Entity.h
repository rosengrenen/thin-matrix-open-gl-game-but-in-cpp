#ifndef ENTITY_H
#define ENTITY_H

#include <glm\vec3.hpp>

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
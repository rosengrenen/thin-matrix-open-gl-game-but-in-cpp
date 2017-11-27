#pragma once

#include <glm\gtc\matrix_transform.hpp>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>

#include "TexturedModel.h"

class Entity
{
public:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	float m_scale;
	int textureIndex;
	TexturedModel texturedModel;
public:
	Entity(const TexturedModel& texturedModel, const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), float scale = 1.0f, int textureIndex = 0);

	void move(float dx, float dy, float dz);

	void rotate(float dx, float dy, float dz);

	const glm::mat4 getModelMatrix() const;

	const float getTextureXOffset() const;

	const float getTextureYOffset() const;
};
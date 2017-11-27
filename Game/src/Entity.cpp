#include "Entity.h"

Entity::Entity(const TexturedModel& texturedModel, const glm::vec3& position, const glm::vec3& rotation, float scale, int textureIndex) : texturedModel(texturedModel), m_position(position), m_rotation(rotation), m_scale(scale), textureIndex(textureIndex)
{ }

void Entity::move(float dx, float dy, float dz)
{
	m_position.x += dx;
	m_position.y += dy;
	m_position.z += dz;
}

void Entity::rotate(float dx, float dy, float dz)
{
	m_rotation.x += dx;
	m_rotation.y += dy;
	m_rotation.z += dz;
}

const glm::mat4 Entity::getModelMatrix() const
{
	// Translate
	glm::mat4 transformation = glm::translate(glm::mat4(1), m_position);
	// RotateX
	transformation = glm::rotate(transformation, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
	// RotateY
	transformation = glm::rotate(transformation, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
	// RotateZ
	transformation = glm::rotate(transformation, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
	// Scale
	return glm::scale(transformation, glm::vec3(m_scale));
}

const float Entity::getTextureXOffset() const
{
	int column = textureIndex % texturedModel.texture.numberOfRows;
	return static_cast<float>(column) / texturedModel.texture.numberOfRows;
}

const float Entity::getTextureYOffset() const
{
	int row = textureIndex / texturedModel.texture.numberOfRows;
	return static_cast<float>(row) / texturedModel.texture.numberOfRows;
}
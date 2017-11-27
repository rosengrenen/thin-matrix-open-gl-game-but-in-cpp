#pragma once

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>

#include "Texture.h"

class GuiTexture
{
private:
public:
	GLuint texture;
	glm::vec2 position;
	glm::vec2 scale;
public:
	GuiTexture(GLuint texture, const glm::vec2& position, const glm::vec2& scale) : texture(texture), position(position), scale(scale)
	{ }

	glm::mat4 transformationMatrix()
	{
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), { position, 0.0f });
		return glm::scale(matrix, { scale, 1.0f });
	}
};
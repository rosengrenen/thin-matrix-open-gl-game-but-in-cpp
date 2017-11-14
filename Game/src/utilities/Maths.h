#ifndef MATHS_H
#define MATHS_H

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <glm\matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "..\entities\Camera.h"

class Maths
{
public:
	static glm::mat4 createTransformationMatrix(const glm::vec3 translation, const glm::vec3 rotation, const float scale)
	{
		// Translate
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
		// Rotate X
		matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		// Rotate Y
		matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		// Rotate Z
		matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		// Scale
		matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

		return matrix;
	}
};

#endif
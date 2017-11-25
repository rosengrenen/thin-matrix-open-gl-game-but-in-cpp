#pragma once

class Light
{
public:
	glm::vec3 position;
	glm::vec3 colour;
	glm::vec3 attenuation;
public:
	Light(const glm::vec3& position, const glm::vec3& colour, const glm::vec3& attenuation = glm::vec3(1.0, 0.0, 0.0)) : position(position), colour(colour), attenuation(attenuation)
	{ }
};
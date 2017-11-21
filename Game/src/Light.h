#pragma once

class Light
{
public:
	glm::vec3 position;
	glm::vec3 colour;
public:
	Light(const glm::vec3& position, const glm::vec3& colour) : position(position), colour(colour)
	{ }
};
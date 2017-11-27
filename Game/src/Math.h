#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

class Math
{
public:
	static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
};
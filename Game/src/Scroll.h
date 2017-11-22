#pragma once

#include <GLFW\glfw3.h>
#include <glm\vec2.hpp>

class Scroll
{
private:
	static GLFWwindow* m_window;
	static double m_xoffset;
	static double m_yoffset;
public:
	static void init(GLFWwindow* window);

	static void update();

	static glm::vec2 getOffset();

	static float getOffsetX();

	static float getOffsetY();
private:
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
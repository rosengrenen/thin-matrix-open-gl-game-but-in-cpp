#include "Scroll.h"

#include <iostream>

GLFWwindow* Scroll::m_window = nullptr;
double Scroll::m_xoffset = 0;
double Scroll::m_yoffset = 0;

void Scroll::init(GLFWwindow* window)
{
	m_window = window;
	glfwSetScrollCallback(m_window, scrollCallback);
}

void Scroll::update()
{
	m_xoffset = 0;
	m_yoffset = 0;
}

glm::vec2 Scroll::getOffset()
{
	return glm::vec2(m_xoffset, m_yoffset);
}

float Scroll::getOffsetX()
{
	return m_xoffset;
}

float Scroll::getOffsetY()
{
	return m_yoffset;
}
void Scroll::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_xoffset += xoffset;
	m_yoffset += yoffset;
}
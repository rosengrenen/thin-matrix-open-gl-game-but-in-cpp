#include "Mouse.h"

std::array<bool, Mouse::NUM_KEYS> Mouse::m_lastKeys;
GLFWwindow* Mouse::m_window = nullptr;
double Mouse::m_xpos = 0.0;
double Mouse::m_ypos = 0.0;

void Mouse::init(GLFWwindow* window)
{
	m_window = window;
}

void Mouse::update()
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		m_lastKeys[i] = getKey(i);
	}
	glfwGetCursorPos(m_window, &m_xpos, &m_ypos);
}

bool Mouse::getKey(int keyCode)
{
	return glfwGetMouseButton(m_window, keyCode);
}

bool Mouse::getKeyDown(int keyCode)
{
	return getKey(keyCode) && !m_lastKeys.at(keyCode);
}

bool Mouse::getKeyUp(int keyCode)
{
	return !getKey(keyCode) && m_lastKeys.at(keyCode);
}

glm::vec2 Mouse::getOffset()
{
	return glm::vec2(getPos().x - m_xpos, getPos().y - m_ypos);
}

float Mouse::getOffsetX()
{
	return getPos().x - m_xpos;
}

float Mouse::getOffsetY()
{
	return m_ypos - getPos().y;
}

glm::vec2 Mouse::getPos()
{
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	return glm::vec2(x, y);
}

float Mouse::getPosX()
{
	return getPos().x;
}

float Mouse::getPosY()
{
	return getPos().y;
}
#include "Keyboard.h"

std::array<bool, Keyboard::NUM_KEYS> Keyboard::m_lastKeys;
GLFWwindow* Keyboard::m_window = nullptr;

void Keyboard::init(GLFWwindow* window)
{
	m_window = window;
}

void Keyboard::update() // Need to be called every frame
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		m_lastKeys[i] = getKey(i);
	}
}

bool Keyboard::getKey(int keyCode)
{
	return glfwGetKey(m_window, keyCode);
}

bool Keyboard::getKeyDown(int keyCode)
{
	return getKey(keyCode) && !m_lastKeys.at(keyCode);
}

bool Keyboard::getKeyUp(int keyCode)
{
	return !getKey(keyCode) && m_lastKeys.at(keyCode);
}
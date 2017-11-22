#pragma once

#include <GLFW\glfw3.h>

#include <array>

struct Key
{
	int scancode = 0;
	int action = 0;
	int mods = 0;
};

class Keyboard
{
private:
	std::array<Key, 348> m_keys;
public:
	Keyboard()
	{ }
	void setState(int key, int scancode, int action, int mods)
	{
		m_keys.at(key).scancode = scancode;
		m_keys.at(key).action = action;
		m_keys.at(key).mods = mods;
	}

	bool isPressed(int key)
	{
		return m_keys.at(key).action == GLFW_PRESS || isRepeated(key);
	}

	bool isReleased(int key)
	{
		return m_keys.at(key).action == GLFW_RELEASE;
	}

	bool isRepeated(int key)
	{
		return m_keys.at(key).action == GLFW_REPEAT;
	}
};
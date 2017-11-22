#pragma once

#include <GLFW\glfw3.h>

#include <array>

class Keyboard
{
private:
	static constexpr int NUM_KEYS = 348;
	static std::array<bool, NUM_KEYS> m_lastKeys;
	static GLFWwindow* m_window;
private:
	Keyboard() = delete;
public:
	static void init(GLFWwindow* window);

	static void update();

	static bool getKey(int keyCode);

	static bool getKeyDown(int keyCode);

	static bool getKeyUp(int keyCode);
};
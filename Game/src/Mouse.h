#pragma once

#include <array>
#include <glm\vec2.hpp>
#include <GLFW\glfw3.h>

class Mouse
{
private:
	static constexpr int NUM_KEYS = 8;
	static std::array<bool, NUM_KEYS> m_lastKeys;
	static GLFWwindow* m_window;
	static double m_xpos;
	static double m_ypos;
private:
	Mouse() = delete;
public:
	static void init(GLFWwindow* window);

	static void update();

	static bool getKey(int keyCode);

	static bool getKeyDown(int keyCode);

	static bool getKeyUp(int keyCode);

	static glm::vec2 getOffset();

	static float getOffsetX();

	static float getOffsetY();

	static glm::vec2 getPos();

	static float getPosX();

	static float getPosY();
};
#pragma once

#include <string>

#include <GLFW\glfw3.h>

#include <glm\vec2.hpp>

#include "Keyboard.h"
#include "Mouse.h"

class Window
{
private:
	GLFWwindow* m_window;
public:
	Window(int width, int height, const char* name)
	{
		m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
		makeCurrentContext();
	}

	void makeCurrentContext()
	{
		glfwMakeContextCurrent(m_window);
	}

	void swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	GLFWwindow* getWindow()
	{
		return m_window;
	}

	float getAspectRatio()
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		return static_cast<float>(width) / height;
	}

	bool shouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	void setCursorPos(float x, float y)
	{
		glfwSetCursorPos(m_window, x, y);
	}
};
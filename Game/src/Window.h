#pragma once

#include <string>

#include <GLFW\glfw3.h>

class Window
{
private:
	GLFWwindow* m_window;
public:
	Window(int width, int height, const char* name)
	{
		m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
		if (!m_window)
		{
			glfwTerminate();
			return;
		}
		glfwSetWindowUserPointer(m_window, this);

		glfwSetKeyCallback(m_window, onKey);
		glfwSetMouseButtonCallback(m_window, onMouse);
		glfwSetScrollCallback(m_window, onScroll);

		makeContext();
	}

	void makeContext()
	{
		glfwMakeContextCurrent(m_window);
	}

	GLFWwindow* getWindow()
	{
		return m_window;
	}
private:
	static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* obj = (Window*)glfwGetWindowUserPointer(window);
	}

	static void onMouse(GLFWwindow* window, int button, int action, int mods)
	{

	}

	static void onScroll(GLFWwindow* window, double xoffset, double yoffset)
	{

	}
};
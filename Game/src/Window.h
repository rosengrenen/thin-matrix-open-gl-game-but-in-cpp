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
	Keyboard m_keyboard;
	Mouse m_mouse;
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

	void swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	// Temporary
	GLFWwindow* getWindow()
	{
		return m_window;
	}

	bool isKeyPressed(int key)
	{
		return m_keyboard.isPressed(key);
	}

	bool isKeyReleased(int key)
	{
		return m_keyboard.isReleased(key);
	}

	bool isKeyRepeated(int key)
	{
		return m_keyboard.isRepeated(key);
	}

	bool isMousePressed(int key)
	{

	}

	bool isMouseReleased(int key)
	{

	}

	bool isMouseRepeated(int key)
	{

	}

	void prepare()
	{
		m_mouse.prepare(m_window);
	}

	float mouseOffsetX()
	{
		return m_mouse.getOffset().x;
	}

	float mouseOffsetY()
	{
		return m_mouse.getOffset().y;
	}

	glm::vec2 getMousePos()
	{
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		return glm::vec2(xpos, ypos);
	}

	float getAspectRatio()
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);
		return static_cast<float>(width) / height;
	}
private:
	static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* instance = (Window*) glfwGetWindowUserPointer(window);
		instance->m_keyboard.setState(key, scancode, action, mods);
	}

	static void onMouse(GLFWwindow* window, int button, int action, int mods)
	{
		Window* instance = (Window*) glfwGetWindowUserPointer(window);
	}

	static void onScroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* instance = (Window*) glfwGetWindowUserPointer(window);
	}
};
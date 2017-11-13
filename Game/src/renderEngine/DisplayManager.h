#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <GLFW\glfw3.h>

class DisplayManager
{
private:
	GLFWwindow* m_window;
public:
	DisplayManager() : DisplayManager(800, 600, "Default Title")
	{ }
	
	DisplayManager(const int width, const int height, const char* title)
	{
		/* Initialize the library */
		if (!glfwInit())
			return;

		/* Create a windowed mode window and its OpenGL context */
		m_window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!m_window)
		{
			glfwTerminate();
			return;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_window);

	}

	void update()
	{

	}

	void swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void close()
	{
		glfwDestroyWindow(m_window);
	}

	bool shouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	void setTitle(const char* title)
	{
		glfwSetWindowTitle(m_window, title);
	}

	void hide()
	{
		glfwHideWindow(m_window);
	}

	void show()
	{
		glfwShowWindow(m_window);
	}

	void setPosition(const int left, const int top)
	{
		glfwSetWindowPos(m_window, left, top);
	}
	void setSize(const int width, const int height)
	{
		glfwSetWindowSize(m_window, width, height);
	}
};

#endif
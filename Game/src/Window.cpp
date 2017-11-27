#include "Window.h"

GLFWwindow* Window::m_window;

void Window::create(int width, int height, const char* name)
{
	m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	hide();
	makeContext();
}

GLFWwindow* Window::getWindow()
{
	return m_window;
}

void Window::makeContext()
{
	glfwMakeContextCurrent(m_window);
}

void Window::update()
{
	glfwSwapBuffers(m_window);
}

float Window::getAspectRatio()
{
	glm::vec2 size = getSize();
	return static_cast<float>(size.x) / size.y;
}

void Window::clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setCursorPos(float x, float y)
{
	glfwSetCursorPos(m_window, x, y);
}

bool Window::isOpen()
{
	return !glfwWindowShouldClose(m_window);
}

void Window::destroy()
{
	glfwDestroyWindow(m_window);
}

glm::vec2 Window::getPosition()
{
	int xpos, ypos;
	glfwGetWindowPos(m_window, &xpos, &ypos);
	return { xpos, ypos };
}

void Window::setPosition(int left, int top)
{
	glfwSetWindowPos(m_window, left, top);
}

glm::vec2 Window::getSize()
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return { width, height };
}

void Window::setSize(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

void Window::setSizeLimits(int minwidth, int minheight, int maxwidth, int maxheight)
{
	glfwSetWindowSizeLimits(m_window, minwidth, minheight, maxwidth, maxheight);
}

void Window::setTitle(const std::string& title)
{
	glfwSetWindowTitle(m_window, title.c_str());
}

void Window::show()
{
	glfwShowWindow(m_window);
}

void Window::hide()
{
	glfwHideWindow(m_window);
}

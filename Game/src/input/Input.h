#ifndef INPUT_H
#define INPUT_H

#include <GLFW\glfw3.h>

#include "Keyboard.h"
#include "Mouse.h"
#include "Scroll.h"

class Input
{
public:
	Keyboard m_keyboard;
	Mouse m_mouse;
	Scroll m_scroll;
	GLFWwindow* window;
public:
	Input()
	{

	}
};

#endif
#pragma once


#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\vec2.hpp>

#include <string>

#include "Keyboard.h"
#include "Mouse.h"

class Window
{
private:
	static GLFWwindow* m_window;
public:
	static void create(int width, int height, const char* name);

	static GLFWwindow* getWindow();

	static void makeContext();

	static void update();

	static float getAspectRatio();

	static void clear(float r, float g, float b);

	//TODO: Move to mouse class
	void setCursorPos(float x, float y);

	static bool isOpen();
	static void destroy();
	static glm::vec2 getPosition();
	static void setPosition(int left, int top);
	static glm::vec2 getSize();
	static void setSize(int width, int height);
	static void setSizeLimits(int minwidth, int minheight, int maxwidth, int maxheight);
	static void setAspectRatio(int width, int height);
	static void setDefault();
	static void setTitle(const std::string& title);
	static void setWindowIcon(int count, GLFWimage* images);
	static glm::vec2 getFramebufferSize();
	static void getFrameSize();
	static void iconify();
	static void restore();
	static void maximize();
	static void show();
	static void hide();
	static void focus();
	static void getMonitor();
	static void setMonitor();
	static void pollEvents();
	static void waitEvents();
	static void waitEventsTimeout();
	static void postEmptyEvent();

	static bool isFocuses();
	static bool isIconified();
	static bool isMaximised();
	static bool isVisible();
	static bool isResizable();
	static bool isDecorated();
	static bool isFloating();

	static std::string clientApi();
	static std::string contextCreationApi();
	static int majorVersion();
	static int minorVersion();
	static int revision();

	static bool isForwardCompatible();
	static bool isDebugEnabled();
	static std::string getProfile();
	static std::string getRobustness();

	static void enableVerticalSync();
	static void disableVerticalSync();

	static void framebufferDetails();

	/* CALLBACKS */
	static void windowPosFunc();
	static void windowSizeFunc();
	static void windowCloseFunc();
	static void windowRefreshFunc();
	static void windowFocusFunc();
	static void windowIconifyFunc();
	static void framebufferSizeFunc();
};
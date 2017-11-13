#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include "shaders/StaticShader.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"

int main(void)
{
	DisplayManager window(800, 600, "Window Title");

	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	Loader loader;
	Renderer renderer;
	std::vector<float> vertexes {
		-0.95f,  0.95f, 1.0f,
		 0.95f,  0.95f, 1.0f,
		 0.95f, -0.95f, 1.0f,
		-0.95f, -0.95f, 1.0f
	};
	std::vector<int> indices {
		0, 3, 1,
		1, 3, 2
	};
	RawModel triangle = loader.loadToVAO(vertexes, indices);
	//SomeStuff texture = loader.loadTexture(std::string& fileName);
	StaticShader triangleProgram("res/shaders/triangle.shader");
	triangleProgram.use();

	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{
		/* Render here */
		renderer.prepare();

		triangleProgram.use();
		renderer.render(triangle);
		triangleProgram.stop();

		/* Swap front and back buffers */
		window.swapBuffers();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
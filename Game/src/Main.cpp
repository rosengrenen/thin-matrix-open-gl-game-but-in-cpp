#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include "models\RawModel.h"
#include "models\TexturedModel.h"
#include "textures\ModelTexture.h"
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
		-0.95f,  0.95f, 0.0f, // V0
		-0.95f, -0.95f, 0.0f, // V1
		 0.95f, -0.95f, 0.0f, // V2
		 0.95f,  0.95f, 0.0f, // V3
	};
	std::vector<float> texCoords {
		0, 0, // V0
		1, 0, // V1
		1, 1, // V2
		0, 1, // V3
	};
	std::vector<int> indices {
		0, 1, 3,
		3, 1, 2,
	};
	RawModel model = loader.loadToVAO(vertexes, texCoords, indices);
	ModelTexture texture = loader.loadTexture("wall.jpg");
	TexturedModel texturedModel(model, texture);
	StaticShader triangleProgram("res/shaders/triangle.shader");
	triangleProgram.use();

	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{
		/* Render here */
		renderer.prepare();

		triangleProgram.use();
		renderer.render(texturedModel);
		triangleProgram.stop();

		/* Swap front and back buffers */
		window.swapBuffers();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
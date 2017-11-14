#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include "entities\Entity.h"
#include "entities\Camera.h"
#include "models\RawModel.h"
#include "models\TexturedModel.h"
#include "textures\ModelTexture.h"
#include "shaders/StaticShader.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
#include "utilities\Maths.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

int main(void)
{
	DisplayManager window(800, 600, "Window Title");

	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	Loader loader;
	std::vector<float> vertexes {
		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f
	};
	std::vector<float> texCoords {
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0
	};
	std::vector<int> indices {
		0,1,3,
		3,1,2,
		4,5,7,
		7,5,6,
		8,9,11,
		11,9,10,
		12,13,15,
		15,13,14,
		16,17,19,
		19,17,18,
		20,21,23,
		23,21,22
	};
	RawModel model = loader.loadToVAO(vertexes, texCoords, indices);
	ModelTexture texture(loader.loadTexture("wall.jpg"));
	TexturedModel staticModel(model, texture);
	StaticShader shader("res/shaders/triangle.shader");
	Renderer renderer(shader);
	Entity entity(staticModel, glm::vec3(0), glm::vec3(0), 0.8f);

	Camera camera;
	GLFWwindow* gWindow = window.getWindow();

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{
		camera.processInput(gWindow, deltaTime);
		entity.rotate(glm::vec3(0, 1, 0));
		/* Render here */
		renderer.prepare();
		GLCall(shader.use());
		shader.loadViewMatrix(camera);
		GLCall(renderer.render(entity, shader));
		GLCall(shader.stop());

		/* Swap front and back buffers */
		window.swapBuffers();

		/* Poll for and process events */
		glfwPollEvents();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}
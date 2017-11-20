#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\constants.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb_image.h"

#include "Camera.h"
#include "Entity.h"
#include "Light.h"
#include "Loader.h"
#include "Renderer.h"
#include "StaticShader.h"
#include "Window.h"

#pragma region GL_DEBUG_TOOLS
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
#pragma endregion

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* Create a windowed mode window and its OpenGL context 
	 * Keyboard and Mouse classes should be separate to the Window class, they'll just take the Window pointer as a constructor argument
	 */
	Window window(800, 600, "New window");


	#pragma region GLEW_INIT
	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	#pragma endregion

	Loader loader;

	StaticShader shader("res/shaders/triangle.shader");

	//Model cubeModel = loader.loadToVao(vertices, texCoords, normals, indices);
	Model stall = loader.loadObj("dragon");

	Texture cubeTexture("res/textures/stallTexture.png");
	cubeTexture.shineDamper = 3;
	cubeTexture.reflectivity = 0.3f;

	Entity cube(
		stall,
		cubeTexture,
		glm::vec3(0, 0, -50),
		glm::vec3(0, 0, 0),
		1.8f
	);

	Light light(glm::vec3(0, 0, 20), glm::vec3(1, 1, 1));

	Camera camera(glm::vec3(0), 0, -90);

	Renderer renderer;

	// !! WINDOW <- SCRAP THAT -> FPS COUNTER CLASS?
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;

	double mouseSensitivity = 0.3;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float movementSpeed = 1.0f;

	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{
		// Method naming should be changed...
		window.prepare();

		cube.rotate(0, 0.02f, 0);

		/* Rotate camera from mouse input */
		//std::cout << window.mouseOffsetX() << std::endl;
		camera.rotate(window.mouseOffsetY() * mouseSensitivity, window.mouseOffsetX() * mouseSensitivity);

		/* Mouse the camera from keyboard input */
		if (window.isKeyPressed(GLFW_KEY_W))
		{
			camera.moveFront(movementSpeed, 0, movementSpeed);
		}
		else if (window.isKeyPressed(GLFW_KEY_S))
		{
			camera.moveFront(-movementSpeed, 0, -movementSpeed);
		}
		if (window.isKeyPressed(GLFW_KEY_A))
		{
			camera.moveRight(-movementSpeed, 0, -movementSpeed);
		}
		else if (window.isKeyPressed(GLFW_KEY_D))
		{
			camera.moveRight(movementSpeed, 0, movementSpeed);
		}
		if (window.isKeyPressed(GLFW_KEY_SPACE))
		{
			camera.move(0, movementSpeed, 0);
		}
		else if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			camera.move(0, -movementSpeed, 0);
		}
		
		renderer.prepare();

		shader.use();
		shader.setTransformationMatrix(cube.getTransformationMatrix());
		shader.setProjectionMatrix(camera.getProjectionMatrix(window.getAspectRatio()));
		shader.setViewMatrix(camera.getViewMatrix());
		shader.setLight(light);
		shader.setShineVariables(cube.texture.reflectivity, cube.texture.shineDamper);
		renderer.render(cube);

		window.swapBuffers();

		/* Poll for and process events !! WINDOW?? */
		GLCall(glfwPollEvents());
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// FPS !! WINDOW? !! FPS?
		frames++;
		if (currentFrame - frameTime > 1.0f)
		{
			std::cout << "[FPS] " << frames << std::endl;
			frameTime = currentFrame;
			frames = 0;
		}
	}

	glfwTerminate();
	return 0;
}
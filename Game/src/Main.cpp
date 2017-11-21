#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <ctime>

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
#include "MasterRenderer.h"
#include "EntityRenderer.h"
#include "EntityShader.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
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

	

	//Model cubeModel = loader.loadToVao(vertices, texCoords, normals, indices);
	Model model = loader.loadObj("stall");
	Texture texture("res/textures/stallTexture.png");
	texture.shineDamper = 3;
	texture.reflectivity = 0.3f;
	TexturedModel texturedModel(model, texture);

	Model fernModel = loader.loadObj("grassModel");
	Texture fernTexture("res/textures/grassTexture.png");
	fernTexture.shineDamper = 10;
	fernTexture.reflectivity = 0.2f;
	fernTexture.hasTransparency = true;
	fernTexture.useFakeLighting = true;
	TexturedModel fern(fernModel, fernTexture);

	Model treeModel = loader.loadObj("tree");
	Texture treeTex("res/textures/tree.png");
	treeTex.reflectivity = 2.0f;
	treeTex.shineDamper = 10.0f;
	TexturedModel tree(treeModel, treeTex);

	std::vector<Entity> entities;
	srand((unsigned int) time(0));
	for (int i = 0; i < 100; i++)
	{
		glm::vec3 position((float) (rand() % 100 - 50), rand() % 100 - 50, (rand() % 100) * -3);
		glm::vec3 rotation((float) (rand() % 180), rand() % 180, rand() % 180);
		float scale = static_cast<float>(rand() % 100) / 100.0f + 0.5f;
		entities.push_back(Entity(texturedModel, position, rotation, scale));
	}

	std::vector<Entity> ferns;
	for (int i = 0; i < 200; i++)
	{
		glm::vec3 position((float) (rand() % 1400) + 100.0f, 0.0f, (rand() % 600) + 100.0f);
		glm::vec3 rotation(0.0f, rand() % 180, 0.0f);
		float scale = 12.0f;
		entities.push_back(Entity(fern, position, rotation, scale));
	}

	Light light(glm::vec3(2000, 2000, 2000), glm::vec3(1, 1, 1));

	Camera camera(glm::vec3(0), 0, -90);

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer(terrainShader);

	EntityShader entityShader;
	EntityRenderer entityRenderer(entityShader);

	MasterRenderer renderer(entityRenderer, terrainRenderer, entityShader, terrainShader);

	Texture container("res/textures/container.jpg");
	Texture grass("res/textures/grass.png");
	Terrain terrain(0, 0, loader, container);
	Terrain terrain2(1, 0, loader, grass);

	// !! WINDOW <- SCRAP THAT -> FPS COUNTER CLASS?
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;

	double mouseSensitivity = 0.3;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float movementSpeed = 1.0f;
	glfwSwapInterval(0);
	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{
		// Method naming should be changed...
		window.prepare();

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

		for (int i = 0; i < entities.size(); i++)
		{
			renderer.processEntity(entities.at(i));
		}
		for (int i = 0; i < ferns.size(); i++)
		{
			renderer.processEntity(ferns.at(i));
		}

		renderer.processTerrains(terrain);
		renderer.processTerrains(terrain2);

		renderer.render(light, camera);
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
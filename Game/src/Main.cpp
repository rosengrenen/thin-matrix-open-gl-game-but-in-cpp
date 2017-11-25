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
#include "TerrainTexture.h"
#include "TerrainTexturePack.h"
#include "Player.h"
#include "GuiTexture.h"
#include "GuiRenderer.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "Scroll.h"

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

	Window window(800, 600, "New window");

	#pragma region GLEW_INIT
	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	#pragma endregion

	/* Create a windowed mode window and its OpenGL context
	* Keyboard and Mouse classes should be separate to the Window class, they'll just take the Window pointer as a constructor argument
	*/


	Keyboard::init(window.getWindow());
	Mouse::init(window.getWindow());
	Scroll::init(window.getWindow());

	#pragma region TERRAIN TEXTURE
	TerrainTexture bgTexture(Texture("grassy2.png").getID());
	TerrainTexture rTexture(Texture("mud.png").getID());
	TerrainTexture gTexture(Texture("grassFlowers.png").getID());
	TerrainTexture bTexture(Texture("path.png").getID());
	TerrainTexturePack texturePack(bgTexture, rTexture, gTexture, bTexture);
	TerrainTexture blendMap(Texture("blendMap.png").getID());

	//TODO: pass heightmap texture instead of string
	//TODO: Options to image creation to decide number of channels
	Terrain terrain(0, 0, texturePack, blendMap, "heightmap.png");
	#pragma endregion

	#pragma region FERN
	Model fernModel = Loader::loadObj("fern");
	Texture fernTex("fernAtlas.png");
	fernTex.hasTransparency = true;
	fernTex.reflectivity = 0.3f;
	fernTex.shineDamper = 7.0f;
	fernTex.numRows = 2;
	TexturedModel fern(fernModel, fernTex);

	srand(time(0));
	std::vector<Entity> ferns;
	for (int i = 0; i < 50; i++)
	{
		float x = rand() % 700 + 50.0f;
		float z = rand() % 700 + 50.0f;
		glm::vec3 position(x, terrain.getHeightOfTerrain(x, z), z);
		ferns.push_back(Entity(fern, position, glm::vec3(0, static_cast<float>(rand() % 180), 0), 2.0f, rand() % 4));
	}
	#pragma endregion

	#pragma region PLAYER
	Model playerModel = Loader::loadObj("person");
	Texture playerTexture("playerTexture.png");
	playerTexture.shineDamper = 10.0f;
	playerTexture.reflectivity = 0.7f;
	TexturedModel playerTM(playerModel, playerTexture);

	Player player(playerTM, glm::vec3(800.0f, 0, 800.0f), glm::vec3(0), 1.0f);
	#pragma endregion
	
	Light light(glm::vec3(100, 200, 700), glm::vec3(1, 1, 1));
	Camera camera(player, glm::vec3(800.0f, 12.0f, 805.0f), 30, 0);

	std::vector<GuiTexture> guis;
	GuiTexture gui(Loader::loadTexture("socuwan.png").id, { 0.5f,0.5f }, { 0.25f, 0.25f });
	guis.push_back(gui);
	GuiTexture gui2(Loader::loadTexture("thinmatrix.png").id, { 0.4f,0.6f }, { 0.25f, 0.25f });
	guis.push_back(gui2);

	GuiRenderer guiRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer(terrainShader);

	EntityShader entityShader;
	EntityRenderer entityRenderer(entityShader);

	MasterRenderer renderer(entityRenderer, terrainRenderer, entityShader, terrainShader);

	// !! WINDOW <- SCRAP THAT -> FPS COUNTER CLASS?
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;

	double mouseSensitivity = 0.3;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float movementSpeed = 1.0f;
	glfwSwapInterval(1);
	/* Loop until the user closes the window */
	while (!window.shouldClose())
	{

		/* Rotate camera from mouse input */
		camera.zoom(-Scroll::getOffsetY() * 4);
		if (Mouse::getKey(GLFW_MOUSE_BUTTON_RIGHT))
		{
			camera.rotate(-Mouse::getOffsetY(), Mouse::getOffsetX());
		}

		/* Mouve the player from keyboard input */
		if (Keyboard::getKey(GLFW_KEY_A))
		{
			//camera.moveRight(-movementSpeed, 0, -movementSpeed);
			player.rotateACW();
		}
		else if (Keyboard::getKey(GLFW_KEY_D))
		{
			//camera.moveRight(movementSpeed, 0, movementSpeed);
			player.rotateCW();
		}
		if (Keyboard::getKey(GLFW_KEY_W))
		{
			//camera.moveFront(movementSpeed, 0, movementSpeed);
			player.moveFront();
		}
		else if (Keyboard::getKey(GLFW_KEY_S))
		{
			//camera.moveFront(-movementSpeed, 0, -movementSpeed);
			player.moveBack();
		}
		if (Keyboard::getKey(GLFW_KEY_SPACE))
		{
			//camera.move(0, movementSpeed, 0);
			player.jump();
		}
		Mouse::update();
		Keyboard::update();
		Scroll::update();

		player.moveP(terrain);
		for (int i = 0; i < ferns.size(); i++)
		{
			renderer.processEntity(ferns.at(i));
		}

		camera.calcCamPos();

		/*for (int i = 0; i < grass.size(); i++)
		{
			renderer.processEntity(grass.at(i));
		}
		for (int i = 0; i < ferns.size(); i++)
		{
			renderer.processEntity(ferns.at(i));
		}
		for (int i = 0; i < trees.size(); i++)
		{
			renderer.processEntity(trees.at(i));
		}*/
		renderer.processEntity(player);

		renderer.processTerrains(terrain);
		//renderer.processTerrains(terrain2);
		//renderer.processTerrains(terrain3);
		//renderer.processTerrains(terrain4);

		renderer.render(light, camera);
		guiRenderer.render(guis);

		window.swapBuffers();

		/* Poll for and process events !! WINDOW?? */
		glfwPollEvents();
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
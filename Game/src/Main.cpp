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
#include "MousePicker.h"

#include "WaterTile.h"
#include "WaterRenderer.h"
#include "WaterShader.h"
#include "WaterFrameBuffers.h"

#include "Keyboard.h"
#include "Mouse.h"
#include "Scroll.h"

//#include "TextMaster.h"

#include "newFont.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	Window::create(800, 600, "New window");

	#pragma region GLEW_INIT
	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	#pragma endregion

	Keyboard::init(Window::getWindow());
	Mouse::init(Window::getWindow());
	Scroll::init(Window::getWindow());

	std::vector<Entity> entities;
	std::vector<Entity> normalMapEntities;
	std::vector<Terrain> terrains;
	std::vector<Light> lights;
	std::vector<GuiTexture> guis;

	#pragma region TERRAIN TEXTURE
	TerrainTexture bgTexture(Loader::loadTexture2D("grassy2.png"));
	TerrainTexture rTexture(Loader::loadTexture2D("mud.png"));
	TerrainTexture gTexture(Loader::loadTexture2D("grassFlowers.png"));
	TerrainTexture bTexture(Loader::loadTexture2D("path.png"));
	TerrainTexturePack texturePack(bgTexture, rTexture, gTexture, bTexture);
	TerrainTexture blendMap(Loader::loadTexture2D("blendMap.png"));

	//TODO: pass heightmap texture instead of string
	//TODO: Options to image creation to decide number of channels
	Terrain terrain(0, 0, texturePack, blendMap, "heightmap.png");
	#pragma endregion
	terrains.push_back(terrain);

	#pragma region PLAYER
	RawModel playerModel = Loader::loadObj("person");
	Texture playerTexture = Loader::loadTexture2D("playerTexture.png");
	playerTexture.shineDamper = 10.0f;
	playerTexture.reflectivity = 0.7f;
	TexturedModel playerTM(playerModel, playerTexture);

	Player player(playerTM, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0), 1.0f);
	#pragma endregion
	entities.push_back(player);

	Light sun(glm::vec3(400, 200, 400), glm::vec3(0.9f, 0.9f, 0.9f));
	lights.push_back(sun);

	RawModel lampModel = Loader::loadObj("bobbleTree");
	Texture lampTex = Loader::loadTexture2D("bobbleTree.png");
	lampTex.useFakeLighting = false;
	TexturedModel lamp(lampModel, lampTex);

	RawModel barrelModel = Loader::loadNormalObj("boulder");
	Texture barrelTex = Loader::loadTexture2D("boulder.png");
	barrelTex.shineDamper = 10.0f;
	barrelTex.reflectivity = 0.5f;
	barrelTex.m_normalMap = Loader::loadTexture2D("boulderNormal.png").getID();
	TexturedModel barrelTM(barrelModel, barrelTex);

	normalMapEntities.push_back(Entity(barrelTM, glm::vec3(400.0f, 10.0f, 400.0f)));
	//entities.push_back(Entity(barrelTM, glm::vec3(400.0f, 10.0f, 400.0f)));
	srand(time(0));
	for (int i = 0; i < 100; i++)
	{
		float x = rand() % 700 + 50.0f;
		float z = rand() % 700 + 50.0f;
		while (terrain.getHeightOfTerrain(x, z) < 1.0f)
		{
			x = rand() % 700 + 50.0f;
			z = rand() % 700 + 50.0f;
		}
		entities.push_back(Entity(lamp, glm::vec3(x, terrain.getHeightOfTerrain(x, z), z), glm::vec3(0.0f, rand() % 180, 0.0f)));
	}
	Camera camera(player, glm::vec3(400.0f, 12.0f, 405.0f), 30, 0);

	GuiTexture gui(Loader::loadTexture2D("health.png").getID(), { -0.72f,0.88f }, { 0.25f, 0.25f });
	guis.push_back(gui);

	#pragma region RENDERERS
	GuiRenderer guiRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer(terrainShader);

	EntityShader entityShader;
	EntityRenderer entityRenderer(entityShader);

	WaterFrameBuffers fbos;
	WaterRenderer waterRenderer(fbos);

	MasterRenderer renderer(entityRenderer, terrainRenderer, entityShader, terrainShader);
	#pragma endregion

	std::vector<WaterTile> waters;
	WaterTile water(400, 400, 0);
	waters.push_back(water);

	Entity lampEntity(lamp, glm::vec3(0));
	entities.push_back(lampEntity);
	Light lampLight(glm::vec3(0.0), { 10.0f, 10.0f, 0.0f }, { 1, 0.01f, 0.002f });
	lights.push_back(lampLight);

	#pragma region TEXT

	//FontType font(Loader::loadTexture2D("../fonts/verdana.png").getID(), "verdana");

	//GUIText text("Sample text", 1, font, glm::vec2 { 0.0f, 0.0f }, 0.5f, false);

	Font verdana("verdana");

	#pragma endregion

	//GuiTexture reflection(fbos.getReflectionTexture(), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f));
	//guis.push_back(reflection);
	//GuiTexture refraction(fbos.getRefractionTexture(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.25f));
	//guis.push_back(refraction);

	#pragma region FPS
	// !! WINDOW <- SCRAP THAT -> FPS COUNTER CLASS?
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;
	#pragma endregion

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float movementSpeed = 1.0f;
	glfwSwapInterval(1);
	Window::show();
	/* Loop until the user closes the window */
	while (Window::isOpen())
	{
		//Window::clear();
		normalMapEntities.at(0).rotate(0, 0.4f, 0);
		glEnable(GL_CLIP_DISTANCE0);
		/* Rotate camera from mouse input */
		camera.zoom(-Scroll::getOffsetY() * 4);
		if (Mouse::getKey(GLFW_MOUSE_BUTTON_RIGHT))
		{
			camera.rotate(-Mouse::getOffsetY(), Mouse::getOffsetX());
		}

		/* Mouve the player from keyboard input */
		if (Keyboard::getKey(GLFW_KEY_A))
		{
			player.rotateACW();
		}
		else if (Keyboard::getKey(GLFW_KEY_D))
		{
			player.rotateCW();
		}
		if (Keyboard::getKey(GLFW_KEY_W))
		{
			player.moveFront();
		}
		else if (Keyboard::getKey(GLFW_KEY_S))
		{
			player.moveBack();
		}
		if (Keyboard::getKey(GLFW_KEY_SPACE))
		{
			player.jump();
		}
		Mouse::update();
		Keyboard::update();
		Scroll::update();
		player.moveP(terrain);
		camera.calcCamPos();

		// Render to reflection texture
		fbos.bindReflectionFrameBuffer();
		camera.invertPitchAndPlayer();
		camera.calcCamPos();
		renderer.renderScene(entities, normalMapEntities, terrains, lights, camera, glm::vec4(0, 1, 0, -water.getHeight() + 1.0f));
		camera.invertPitchAndPlayer();
		camera.calcCamPos();
		// Render to refraction texture
		fbos.bindRefractionFrameBuffer();
		renderer.renderScene(entities, normalMapEntities, terrains, lights, camera, glm::vec4(0, -1, 0, water.getHeight()));

		// Render to screen
		fbos.unbindCurrentFrameBuffer();
		glDisable(GL_CLIP_DISTANCE0);
		renderer.renderScene(entities, normalMapEntities, terrains, lights, camera, glm::vec4(0, 0, 0, 0));
		waterRenderer.render(waters, camera, sun);
		guiRenderer.render(guis);
		//TextMaster::render();

		Window::update();

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
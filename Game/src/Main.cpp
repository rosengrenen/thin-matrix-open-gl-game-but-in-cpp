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
	Terrain terrain(0, 0, texturePack, blendMap, "ownheightmap.png");
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

	Light light(glm::vec3(100, 200, 700), glm::vec3(0.4f, 0.4f, 0.4f));
	lights.push_back(light);
	lights.push_back(Light({ 450.0f, terrain.getHeightOfTerrain(450, 450) + 14, 450 }, { 3.5f, 0, 0 }, { 1, 0.01f, 0.002f }));
	lights.push_back(Light({ 400.0f, terrain.getHeightOfTerrain(400, 400) + 14, 400 }, { 3.5f, 3.5f, 0 }, { 1, 0.01f, 0.002f }));

	RawModel lampModel = Loader::loadObj("lamp");
	Texture lampTex = Loader::loadTexture2D("lamp.png");
	lampTex.useFakeLighting = true;
	TexturedModel lamp(lampModel, lampTex);

	entities.push_back(Entity(lamp, { 450.0f, terrain.getHeightOfTerrain(450, 450), 450 }));
	entities.push_back(Entity(lamp, { 400.0f, terrain.getHeightOfTerrain(400, 400), 400 }));
	entities.push_back(Entity(lamp, { 350.0f, terrain.getHeightOfTerrain(350, 350), 350 }));
	Camera camera(player, glm::vec3(400.0f, 12.0f, 405.0f), 30, 0);

	GuiTexture gui(Loader::loadTexture2D("health.png").getID(), { -0.72f,0.88f }, { 0.25f, 0.25f });
	guis.push_back(gui);

	#pragma region RENDERERS
	GuiRenderer guiRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer(terrainShader);

	EntityShader entityShader;
	EntityRenderer entityRenderer(entityShader);

	WaterRenderer waterRenderer;

	MasterRenderer renderer(entityRenderer, terrainRenderer, entityShader, terrainShader);
	#pragma endregion

	std::vector<WaterTile> waters;
	WaterTile water(60, 60, 96);
	waters.push_back(water);

	WaterFrameBuffers fbos;

	Entity lampEntity(lamp, glm::vec3(0));
	entities.push_back(lampEntity);
	Light lampLight(glm::vec3(0.0), { 10.0f, 10.0f, 0.0f }, { 1, 0.01f, 0.002f });
	lights.push_back(lampLight);

	GuiTexture reflection(fbos.getReflectionTexture(), glm::vec2(0.5f, 0.5f), glm::vec2(0.25f));
	guis.push_back(reflection);
	GuiTexture refraction(fbos.getRefractionTexture(), glm::vec2(-0.5f, 0.5f), glm::vec2(0.25f));
	guis.push_back(refraction);

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

		renderer.processEntities(entities);
		renderer.processTerrains(terrains);

		// Render to reflection texture
		fbos.bindReflectionFrameBuffer();
		float distance = 2 * (camera.getPosition().y - water.getHeight());
		camera.getPosition().y -= distance;
		camera.invertPitch();
		renderer.renderScene(entities, terrains, lights, camera, glm::vec4(0, 1, 0, -water.getHeight()));
		camera.getPosition().y += distance;
		camera.invertPitch();
		// Render to refraction texture
		fbos.bindRefractionFrameBuffer();
		renderer.renderScene(entities, terrains, lights, camera, glm::vec4(0, -1, 0, water.getHeight()));

		// Render to screen
		fbos.unbindCurrentFrameBuffer();
		glDisable(GL_CLIP_DISTANCE0);
		renderer.renderScene(entities, terrains, lights, camera, glm::vec4(0, -1, 0, 15));
		waterRenderer.render(waters, camera);
		guiRenderer.render(guis);

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
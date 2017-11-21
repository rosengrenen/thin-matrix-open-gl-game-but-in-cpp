#pragma once

#include <unordered_map>
#include "Entity.h"
#include "EntityRenderer.h"
#include "EntityShader.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"

class MasterRenderer
{
private:
	EntityShader entityShader;
	EntityRenderer entityRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer;

	std::unordered_map<TexturedModel, std::vector<Entity>> entities;
	std::vector<Terrain> terrains;
public:
	MasterRenderer(const EntityRenderer& entityRenderer, const TerrainRenderer& terrainRenderer, const EntityShader& entityShader, const TerrainShader& terrainShader) : entityRenderer(entityRenderer), terrainRenderer(terrainRenderer), entityShader(entityShader), terrainShader(terrainShader)
	{ }

	void render(const Light& sun, const Camera& camera)
	{
		entityRenderer.prepare();
		entityShader.use();
		entityShader.setLight(sun);
		entityShader.setViewMatrix(camera.getViewMatrix());
		entityShader.setProjectionMatrix(camera.getProjectionMatrix(800.0f / 600.0f));
		entityRenderer.render(entities);
		entityShader.stop();

		terrainShader.use();
		terrainShader.setLight(sun);
		terrainShader.setViewMatrix(camera.getViewMatrix());
		terrainShader.setProjectionMatrix(camera.getProjectionMatrix(800.0f / 600.0f));
		terrainRenderer.render(terrains);
		terrainShader.stop();

		terrains.clear();
		entities.clear();
	}

	static void enableCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	static void disableCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void processEntity(const Entity& entity)
	{
		if (entities.find(entity.texturedModel) != entities.end())
		{
			entities.at(entity.texturedModel).push_back(entity);
		}
		else
		{
			entities.insert({ entity.texturedModel, std::vector<Entity>{ entity } });
		}
	}

	void processTerrains(Terrain terrain)
	{
		terrains.push_back(terrain);
	}
};
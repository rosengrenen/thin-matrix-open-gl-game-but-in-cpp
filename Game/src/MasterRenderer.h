#pragma once

#include <glm\vec3.hpp>

#include <unordered_map>
#include "Entity.h"
#include "EntityRenderer.h"
#include "EntityShader.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"
#include "SkyboxRenderer.h"
#include "NormalMappingRenderer.h"

class MasterRenderer
{
private:
	EntityShader entityShader;
	EntityRenderer entityRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer;

	SkyboxRenderer skyboxRenderer;

	NormalMappingShader normalShader;
	NormalMappingRenderer normalRenderer;

	std::unordered_map<TexturedModel, std::vector<Entity>> entities;
	std::unordered_map<TexturedModel, std::vector<Entity>> normalEntities;
	std::vector<Terrain> terrains;
	glm::vec3 m_skyColour;
public:
	MasterRenderer(const EntityRenderer& entityRenderer, const TerrainRenderer& terrainRenderer, const EntityShader& entityShader, const TerrainShader& terrainShader) : 
		entityRenderer(entityRenderer), 
		terrainRenderer(terrainRenderer), 
		entityShader(entityShader), 
		terrainShader(terrainShader),
		m_skyColour(glm::vec3(0.5f, 0.5f, 0.5f))
	{ }

	void renderScene(const std::vector<Entity>& entities, const std::vector<Entity>& normalEntities, const std::vector<Terrain>& terrains, const std::vector<Light>& lights, Camera& camera, const glm::vec4& clipPlane)
	{
		for (Entity e : entities)
		{
			processEntity(e);
		}
		for (Entity e : normalEntities)
		{
			processNormalEntity(e);
		}
		for (Terrain t : terrains)
		{
			processTerrain(t);
		}
		render(lights, camera, clipPlane);
	}

	void render(const std::vector<Light>& lights, Camera& camera, const glm::vec4& clipPlane)
	{
		prepare();
		entityShader.use();
		entityShader.setClipPlane(clipPlane);
		entityShader.setSkyColour(m_skyColour.x, m_skyColour.y, m_skyColour.z);
		entityShader.setLight(lights);
		entityShader.setViewMatrix(camera.getViewMatrix());
		entityShader.setProjectionMatrix(camera.getProjectionMatrix());
		entityRenderer.render(entities);
		entityShader.stop();

		normalShader.use();
		normalShader.setClipPlane(clipPlane);
		normalShader.setSkyColour(m_skyColour.x, m_skyColour.y, m_skyColour.z);
		normalShader.setLights(lights);
		normalShader.setViewMatrix(camera.getViewMatrix());
		normalShader.setProjectionMatrix(camera.getProjectionMatrix());
		normalRenderer.render(normalEntities);
		normalShader.stop();

		terrainShader.use();
		terrainShader.setClipPlane(clipPlane);
		terrainShader.setSkyColour(m_skyColour.x, m_skyColour.y, m_skyColour.z);
		terrainShader.setLights(lights);
		terrainShader.setViewMatrix(camera.getViewMatrix());
		terrainShader.setProjectionMatrix(camera.getProjectionMatrix());
		terrainRenderer.render(terrains);
		terrainShader.stop();

		entities.clear();
		normalEntities.clear();
		terrains.clear();

		skyboxRenderer.render(camera, m_skyColour.x, m_skyColour.y, m_skyColour.z);
	}

	void prepare()
	{
		glClearColor(m_skyColour.x, m_skyColour.y, m_skyColour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	void processNormalEntity(const Entity& normalEntity)
	{
		if (normalEntities.find(normalEntity.texturedModel) != normalEntities.end())
		{
			normalEntities.at(normalEntity.texturedModel).push_back(normalEntity);
		}
		else
		{
			normalEntities.insert({ normalEntity.texturedModel, std::vector<Entity>{ normalEntity } });
		}
	}

	void processTerrain(Terrain terrain)
	{
		terrains.push_back(terrain);
	}

	void processTerrains(const std::vector<Terrain>& terrains)
	{
		for (Terrain t : terrains)
		{
			processTerrain(t);
		}
	}

	void skyColour(float r, float g, float b)
	{
		m_skyColour = glm::vec3(r, g, b);
	}
};
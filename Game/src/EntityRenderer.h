#pragma once

#include <GL\glew.h>
#include <glm\mat4x4.hpp>

#include <unordered_map>

#include "Entity.h"
#include "EntityShader.h"
#include "TexturedModel.h"

class EntityRenderer
{
private:
	EntityShader m_shader;

	void bindTexturedModel(const TexturedModel& model);

	void unbindTexturedModel();

	void prepareInstance(const Entity& entity);
	
	void enableCulling();

	void disableCulling();
public:
	EntityRenderer(const EntityShader shader);

	void render(std::unordered_map<TexturedModel, std::vector<Entity>>& entities);

	void stop();
};
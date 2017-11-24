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

	void bindTexturedModel(const TexturedModel& model)
	{
		glBindVertexArray(model.model.getID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.texture.getID());

		if (model.texture.hasTransparency)
		{
			disableCulling();
		}
		m_shader.setShineVariables(model.texture.reflectivity, model.texture.shineDamper);
		m_shader.setFakeLighting(model.texture.useFakeLighting);
		m_shader.setNumRows(model.texture.numRows);
	}

	void unbindTexturedModel()
	{
		enableCulling();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	void prepareInstance(const Entity& entity)
	{
		m_shader.setModelMatrix(entity.getModelMatrix());
		m_shader.setAtlasOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
	}
	
	void enableCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void disableCulling()
	{
		glDisable(GL_CULL_FACE);
	}
public:
	EntityRenderer(const EntityShader shader) : m_shader(shader)
	{ 
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void render(std::unordered_map<TexturedModel, std::vector<Entity>>& entities)
	{
		for (auto& t : entities)
		{
			bindTexturedModel(t.first);
			int numVertices = t.first.model.numVertices;
			for (Entity& e : t.second)
			{
				prepareInstance(e);
				glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, nullptr);
			}
			unbindTexturedModel();
		}
	}

	void stop()
	{
		glDisable(GL_DEPTH_TEST);
	}
};
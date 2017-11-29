#pragma once

#include <unordered_map>

#include <glm\mat4x4.hpp>

#include "Camera.h"
#include "Entity.h"
#include "NormalMappingShader.h"
#include "Texture.h"
#include "TexturedModel.h"

class NormalMappingRenderer
{
private:
	NormalMappingShader shader;
public:
	void render(std::unordered_map<TexturedModel, std::vector<Entity>> entities)
	{
		for (auto& t : entities)
		{
			prepareTexturedModel(t.first);
			int numVertices = t.first.model.getVertexCount();
			for (Entity& e : t.second)
			{
				prepareInstance(e);
				glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, nullptr);
			}
			unbindTexturedModel();
		}
	}
private:
	void prepareTexturedModel(const TexturedModel& model)
	{
		model.model.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glActiveTexture(GL_TEXTURE0);
		model.texture.bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model.texture.m_normalMap);

		if (model.texture.hasTransparency)
		{
			glDisable(GL_CULL_FACE);
		}
		shader.setNumberOfRows(model.texture.numberOfRows);
		shader.setShineVariables(model.texture.shineDamper, model.texture.reflectivity);
	}

	void unbindTexturedModel()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	void prepareInstance(const Entity& entity)
	{
		shader.setModelMatrix(entity.getModelMatrix());
		shader.setOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
	}
};
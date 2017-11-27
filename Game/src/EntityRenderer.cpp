#include "EntityRenderer.h"

void EntityRenderer::bindTexturedModel(const TexturedModel& model)
{
	model.model.bind();
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	model.texture.bind();

	if (model.texture.hasTransparency)
	{
		disableCulling();
	}
	m_shader.setShineVariables(model.texture.reflectivity, model.texture.shineDamper);
	m_shader.setFakeLighting(model.texture.useFakeLighting);
	m_shader.setNumRows(model.texture.numberOfRows);
}

void EntityRenderer::unbindTexturedModel()
{
	enableCulling();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void EntityRenderer::prepareInstance(const Entity& entity)
{
	m_shader.setModelMatrix(entity.getModelMatrix());
	m_shader.setAtlasOffset(entity.getTextureXOffset(), entity.getTextureYOffset());
}

void EntityRenderer::enableCulling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void EntityRenderer::disableCulling()
{
	glDisable(GL_CULL_FACE);
}
EntityRenderer::EntityRenderer(const EntityShader shader) : m_shader(shader)
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void EntityRenderer::render(std::unordered_map<TexturedModel, std::vector<Entity>>& entities)
{
	for (auto& t : entities)
	{
		bindTexturedModel(t.first);
		int numVertices = t.first.model.getVertexCount();
		for (Entity& e : t.second)
		{
			prepareInstance(e);
			glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, nullptr);
		}
		unbindTexturedModel();
	}
}

void EntityRenderer::stop()
{
	glDisable(GL_DEPTH_TEST);
}
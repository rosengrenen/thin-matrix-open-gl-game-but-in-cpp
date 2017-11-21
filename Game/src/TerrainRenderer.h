#pragma once

#include <vector>

#include "Entity.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"

class TerrainRenderer
{
private:
	TerrainShader shader;
private:
	void bindTerrain(const Terrain& terrain)
	{
		glBindVertexArray(terrain.model.getID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		shader.setShineVariables(terrain.texture.reflectivity, terrain.texture.shineDamper);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain.texture.getID());
	}

	void unbindTerrain()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
public:
	TerrainRenderer(TerrainShader shader) : shader(shader)
	{

	}

	void render(std::vector<Terrain> terrains)
	{
		for (Terrain terrain : terrains)
		{
			bindTerrain(terrain);
			shader.setModelMatrix(terrain.getModelMatrix());
			glDrawElements(GL_TRIANGLES, terrain.model.numVertices, GL_UNSIGNED_INT, nullptr);
			unbindTerrain();
		}
	}
};
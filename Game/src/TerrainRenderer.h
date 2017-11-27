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
		terrain.model.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		bindTexture(terrain);
		shader.setShineVariables(0.05f, 20.0f);


	}

	void bindTexture(Terrain terrain)
	{
		glActiveTexture(GL_TEXTURE0);
		terrain.texturePack.bgTex.texture.bind();
		glActiveTexture(GL_TEXTURE1);
		terrain.texturePack.rTex.texture.bind();
		glActiveTexture(GL_TEXTURE2);
		terrain.texturePack.gTex.texture.bind();
		glActiveTexture(GL_TEXTURE3);
		terrain.texturePack.bTex.texture.bind();
		glActiveTexture(GL_TEXTURE4);
		terrain.blendMap.texture.bind();
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
		shader.connectTextureUnits();
	}

	void render(std::vector<Terrain> terrains)
	{
		for (Terrain terrain : terrains)
		{
			bindTerrain(terrain);
			shader.setModelMatrix(terrain.getModelMatrix());
			glDrawElements(GL_TRIANGLES, terrain.model.getVertexCount(), GL_UNSIGNED_INT, nullptr);
			unbindTerrain();
		}
	}
};
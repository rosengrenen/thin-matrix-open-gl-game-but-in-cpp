#pragma once

#include "Model.h"
#include "WaterShader.h"
#include "WaterTile.h"
#include "Camera.h"

class WaterRenderer
{
private:
	Model m_quad;
	WaterShader m_shader;
public:
	WaterRenderer() : m_quad(Loader::loadToVao({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f }, 2))
	{	}

	void render(const std::vector<WaterTile>& water, const Camera& camera)
	{
		prepareRender(camera);
		for (WaterTile& tile : water)
		{
			shader.loadModelMatrix(modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, quad.numVertices);
		}
		unbind();
	}

	void prepareRender(const Camera& camera)
	{
		shader.use();
		shader.setViewMatrix(camera.getViewMatrix());
		glBindVertexArray(quad.bind);
		glEnableVertexAttribArray(0);
	}

	void unbind()
	{
		GL20.glDisableVertexAttribArray(0);
		GL30.glBindVertexArray(0);
		shader.stop();
	}

	void setUpVAO(Loader loader)
	{
		// Just x and z vectex positions here, y is set to 0 in v.shader
		float [] vertices = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
		quad = loader.loadToVAO(vertices, 2);
	}

}
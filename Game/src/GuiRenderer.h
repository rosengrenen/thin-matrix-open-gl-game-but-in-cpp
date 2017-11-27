#pragma once

#include <vector>

#include "RawModel.h"
#include "Loader.h"
#include "GuiTexture.h"
#include "GuiShader.h"

class GuiRenderer
{
private:
public:
	RawModel quad;
	GuiShader shader;
public:
	GuiRenderer() : quad(RawModel({ -1.0f,  1.0f,-1.0f, -1.0f, 1.0f,  1.0f, 1.0f, -1.0f }, 2))
	{ }

	void render(std::vector<GuiTexture> guis)
	{
		shader.use();
		quad.bind();
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (auto& a : guis)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, a.texture);
			shader.setTransformationMatrix(a.transformationMatrix());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.getVertexCount());
		}
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();
	}
};
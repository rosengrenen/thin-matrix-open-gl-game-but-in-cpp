#pragma once

#include "FontShader.h"
#include "FontType.h"
#include "GUIText.h"

#include <unordered_map>

class FontRenderer
{
private:
	FontShader shader;
public:
	FontRenderer()
	{ }

	void render(std::unordered_map<FontType, std::vector<GUIText>> texts)
	{
		prepare();
		for (auto& t : texts)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, t.first.getTextureAtlas());
			for (GUIText& text : t.second)
			{
				renderText(text);
			}
		}
		endRendering();
	}
private:
	void prepare()
	{ 
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		shader.use();
	}

	void renderText(GUIText text)
	{
		glBindVertexArray(text.getMesh());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		shader.setColour(text.getColour());
		shader.setTranslation(text.getPosition());
		glDrawArrays(GL_TRIANGLES, 0, text.getVertexCount());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void endRendering()
	{
		shader.stop();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
};
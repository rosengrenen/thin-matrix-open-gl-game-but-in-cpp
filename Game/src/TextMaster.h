#pragma once

#include <unordered_map>

#include "FontRenderer.h"
#include "FontType.h"
#include "GUIText.h"
#include "Loader.h"

class TextMaster
{
private:
	static std::unordered_map<FontType, std::vector<GUIText>> texts;
	static FontRenderer renderer;
public:
	static void loadText(GUIText& text)
	{
		FontType font = text.getFont();
		TextMeshData data = font.loadText(text);
		GLuint vao = RawModel(data.getVertexPositions(), data.getTextureCoords()).getID();
		text.setMeshInfo(vao, data.getVertexCount());
		if (texts.find(font) != texts.end())
		{
			texts.at(font).push_back(text);
		}
		else
		{
			texts.insert({ font, std::vector<GUIText> { text } });
		}
	}

	static void render()
	{
		renderer.render(texts);
	}

	static void removeText(GUIText& text)
	{
		if (texts.find(text.getFont()) != texts.end())
		{
			std::vector<GUIText>& textBatch = texts.at(text.getFont());
			textBatch.erase(std::remove(textBatch.begin(), textBatch.end(), text), textBatch.end());
			if (textBatch.empty())
			{
				texts.erase(text.getFont());
			}
		}
	}
};
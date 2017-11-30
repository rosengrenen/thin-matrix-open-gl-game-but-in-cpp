#pragma once

#include "GUIText.h"
#include "TextMeshCreator.h"
#include "TextMeshData.h"

class FontType
{
private:
	int m_textureAtlas;
	TextMeshCreator m_loader;

	/**
	* Creates a new font and loads up the data about each character from the
	* font file.
	*
	* @param textureAtlas
	*            - the ID of the font atlas texture.
	* @param fontFile
	*            - the font file containing information about each character in
	*            the texture atlas.
	*/
public:
	FontType(int textureAtlas, const std::string& fontFile) :
		m_textureAtlas { textureAtlas },
		m_loader { "res/fonts" + fontFile + ".fnt" }
	{ }

	/**
	* @return The font texture atlas.
	*/
	int getTextureAtlas() const
	{
		return m_textureAtlas;
	}

	/**
	* Takes in an unloaded text and calculate all of the vertices for the quads
	* on which this text will be rendered. The vertex positions and texture
	* coords and calculated based on the information from the font file.
	*
	* @param text
	*            - the unloaded text.
	* @return Information about the vertices of all the quads.
	*/
	TextMeshData loadText(GUIText text)
	{
		return m_loader.createTextMesh(text);
	}
};
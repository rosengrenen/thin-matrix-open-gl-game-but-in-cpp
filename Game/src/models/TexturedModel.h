#ifndef TEXTURED_MODEL_H
#define TEXTURED_MODEL_H

#include "RawModel.h"
#include "..\textures\ModelTexture.h"

class TexturedModel
{
private:
	RawModel m_rawModel;
	ModelTexture m_texture;
public:
	TexturedModel(RawModel model, ModelTexture texture) : m_rawModel(model), m_texture(texture)
	{ }
	RawModel getRawModel() const
	{
		return m_rawModel;
	}

	ModelTexture getTexture() const
	{
		return m_texture;
	}
};

#endif
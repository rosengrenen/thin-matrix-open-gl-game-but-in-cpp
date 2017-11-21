#pragma once

#include "TerrainTexture.h"

class TerrainTexturePack
{
public:
	TerrainTexture bgTex;
	TerrainTexture rTex;
	TerrainTexture gTex;
	TerrainTexture bTex;
public:
	TerrainTexturePack(TerrainTexture bg, TerrainTexture r, TerrainTexture g, TerrainTexture b) : bgTex(bg), rTex(r), gTex(g), bTex(b)
	{ }
};
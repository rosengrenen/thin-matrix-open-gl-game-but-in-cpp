#pragma once

#include <GL\glew.h>

#include <string>
#include <vector>
#include <fstream>

#include "RawModel.h"
#include "Texture.h"
#include "TextureData.h"
#include "Utilities.h"

class Loader
{
private:
	Loader() = delete;
public:
	/* LOAD TO TEXTURE */

	static Texture loadTexture2D(const std::string& path);

	static Texture loadCubeMap(std::vector<std::string> paths);

	/* LOAD TO RAW MODEL */

	static RawModel loadObj(const std::string& path);
};
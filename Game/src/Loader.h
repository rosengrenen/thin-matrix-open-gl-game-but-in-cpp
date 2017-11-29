#pragma once

#include <GL\glew.h>

#include <string>
#include <vector>
#include <fstream>

#include "RawModel.h"
#include "Texture.h"
#include "TextureData.h"
#include "Utilities.h"

#include "VertexNM.h"

class Loader
{
private:
	Loader() = delete;

	static void calculateTangents(VertexNM& v0, VertexNM& v1, VertexNM& v2, std::vector<glm::vec2>& textures);

	static VertexNM& processVertex(std::vector<std::string>& vertex, std::vector<VertexNM>& vertices, std::vector<int>& indices);

	static float convertDataToArrays(std::vector<VertexNM>& vertices, std::vector<glm::vec2>& textures,
		std::vector<glm::vec3>& normals, std::vector<float>& verticesArray, std::vector<float>& texturesArray,
		std::vector<float>& normalsArray, std::vector<float>& tangentsArray);

	static VertexNM& dealWithAlreadyProcessedVertex(VertexNM& previousVertex, int newTextureIndex,
		int newNormalIndex, std::vector<int>& indices, std::vector<VertexNM>& vertices);

	static void removeUnusedVertices(std::vector<VertexNM>& vertices);
public:
	/* LOAD TO TEXTURE */

	static Texture loadTexture2D(const std::string& path);

	static Texture loadCubeMap(std::vector<std::string> paths);

	/* LOAD TO RAW MODEL */

	static RawModel loadObj(const std::string& path);

	static RawModel loadNormalObj(const std::string& path);
};
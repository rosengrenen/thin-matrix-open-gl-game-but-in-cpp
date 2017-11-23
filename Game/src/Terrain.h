#pragma once

#include <vector>

#include "Model.h"
#include "Texture.h"
#include "Loader.h"
#include "TerrainTexturePack.h"

class Terrain
{
private:
	const float SIZE = 800.0f;
	int VERTEX_COUNT = 128;
	int MAX_PIXEL_COLOUR = 256 * 256 * 256;
	float MAX_HEIGHT = 200.0f;
public:
	float x;
	float z;
	Model model;
	TerrainTexturePack texturePack;
	TerrainTexture blendMap;
private:
	Model generateTerrain(const std::string& heightMapPath)
	{
		double startTime = glfwGetTime();

		Image texture = Loader::loadTexture(heightMapPath);

		VERTEX_COUNT = texture.height;

		int count = VERTEX_COUNT * VERTEX_COUNT;

		std::vector<float> vertices(count * 3);
		std::vector<float> normals(count * 3);
		std::vector<float> texCoords(count * 2);

		std::vector<int> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

		int vertexPointer = 0;
		for (int i = 0; i < VERTEX_COUNT; i++)
		{
			for (int j = 0; j < VERTEX_COUNT; j++)
			{
				vertices[vertexPointer * 3] = (float) j / ((float) VERTEX_COUNT - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = getHeight(j, i, texture);
				vertices[vertexPointer * 3 + 2] = (float) i / ((float) VERTEX_COUNT - 1) * SIZE;

				glm::vec3 normal = calcNormal(x, z, texture);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;

				texCoords[vertexPointer * 2] = (float) j / ((float) VERTEX_COUNT - 1);
				texCoords[vertexPointer * 2 + 1] = (float) i / ((float) VERTEX_COUNT - 1);

				vertexPointer++;
			}
		}

		int pointer = 0;
		for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
		{
			for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
			{
				int topLeft = (gz * VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;

				int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;

				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		std::cout << "[INFO] Generated terrain in " << glfwGetTime() - startTime << " seconds" << std::endl;

		return Loader::loadToVao(vertices, texCoords, normals, indices);
	}


	float getHeight(int x, int z, const Image& image)
	{
		if (x < 0 || x >= image.height || z < 0 || z >= image.width)
		{
			return 0;
		}
		float rgb = image.getRGB(x, z);
		float r = image.getR(x, z);
		float g = image.getG(x, z);
		float b = image.getB(x, z);
		float normalised = rgb / MAX_PIXEL_COLOUR;
		float range = normalised - 0.5f;
		float height = range * MAX_HEIGHT;
		return height;
	}

	glm::vec3 calcNormal(int x, int z, Image heightMap)
	{
		float heightL = getHeight(x - 1, z, heightMap);
		float heightR = getHeight(x + 1, z, heightMap);
		float heightD = getHeight(x, z - 1, heightMap);
		float heightU = getHeight(x, z + 1, heightMap);
		return glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
	}
public:
	Terrain(int gridX, int gridZ, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap, const std::string& heightMapPath) : x(gridX * SIZE), z(gridZ * SIZE), model(generateTerrain(heightMapPath)), texturePack(texturePack), blendMap(blendMap)
	{ }

	glm::mat4 getModelMatrix() const
	{
		return glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
	}
};
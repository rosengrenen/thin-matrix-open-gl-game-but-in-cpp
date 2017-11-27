#pragma once

#include <vector>

#include "RawModel.h"
#include "Texture.h"
#include "Loader.h"
#include "TerrainTexturePack.h"

#include "Math.h"

class Terrain
{
private:
	const float SIZE = 120.0f;
	int VERTEX_COUNT = 128;
	int MAX_PIXEL_COLOUR = 256 * 256 * 256;
	float MAX_HEIGHT = 200.0f;
	std::vector<float> heights;
public:
	float posX;
	float posZ;
	RawModel model;
	TerrainTexturePack texturePack;
	TerrainTexture blendMap;
private:
	RawModel generateTerrain(const std::string& heightMapPath)
	{
		double startTime = glfwGetTime();

		TextureData texture(heightMapPath);

		VERTEX_COUNT = texture.getHeight();

		int count = VERTEX_COUNT * VERTEX_COUNT;

		std::vector<float> vertices(count * 3);
		std::vector<float> normals(count * 3);
		std::vector<float> texCoords(count * 2);
		heights.resize(count);

		std::vector<int> indices(6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1));

		int vertexPointer = 0;
		for (int z = 0; z < VERTEX_COUNT; z++)
		{
			for (int x = 0; x < VERTEX_COUNT; x++)
			{
				float height = getHeight(x, z, texture);
				vertices[vertexPointer * 3] = (float) x / ((float) VERTEX_COUNT - 1) * SIZE;
				vertices[vertexPointer * 3 + 1] = height;
				vertices[vertexPointer * 3 + 2] = (float) z / ((float) VERTEX_COUNT - 1) * SIZE;

				glm::vec3 normal = calcNormal(x, z, texture);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;

				texCoords[vertexPointer * 2] = (float) x / ((float) VERTEX_COUNT - 1);
				texCoords[vertexPointer * 2 + 1] = (float) z / ((float) VERTEX_COUNT - 1);

				heights[vertexPointer] = height;

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

		return RawModel(vertices, texCoords, normals, indices);
	}


	float getHeight(int x, int z, const TextureData& image)
	{
		if (x < 0 || x >= image.getHeight() || z < 0 || z >= image.getWidth())
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

	glm::vec3 calcNormal(int x, int z, TextureData heightMap)
	{
		glm::vec3 positionL(x - 1, getHeight(x - 1, z, heightMap), z);
		glm::vec3 positionR(x + 1, getHeight(x + 1, z, heightMap), z);
		glm::vec3 positionD(x, getHeight(x, z - 1, heightMap), z - 1);
		glm::vec3 positionU(x, getHeight(x, z + 1, heightMap), z + 1);
		return glm::normalize(glm::cross(positionU - positionD, positionR - positionL));
	}
public:
	Terrain(int gridX, int gridZ, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap, const std::string& heightMapPath) : posX(gridX * SIZE), posZ(gridZ * SIZE), model(generateTerrain(heightMapPath)), texturePack(texturePack), blendMap(blendMap)
	{ }

	glm::mat4 getModelMatrix() const
	{
		return glm::translate(glm::mat4(1), glm::vec3(posX, 0, posZ));
	}

	float getHeightOfTerrain(float worldX, float worldZ)
	{
		float terrainX = worldX - posX;
		float terrainZ = worldZ - posZ;
		float gridSquareSize = SIZE / ((float) VERTEX_COUNT - 1);
		int gridX = std::floor(terrainX / gridSquareSize);
		int gridZ = std::floor(terrainZ / gridSquareSize);
		if (gridX >= VERTEX_COUNT - 1 || gridZ >= VERTEX_COUNT - 1 || gridX < 0 || gridZ < 0)
		{
			return 0;
		}
		float xCoord = std::fmod(terrainX, gridSquareSize) / gridSquareSize;
		float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;
		float answer;
		if (xCoord <= zCoord - 1)
		{
			answer = Math::barryCentric(
				glm::vec3(0, getHeight(gridX, gridZ), 0),
				glm::vec3(1, getHeight(gridX + 1, gridZ), 0),
				glm::vec3(0, getHeight(gridX, gridZ + 1), 1),
				glm::vec2(xCoord, zCoord)
			);
		}
		else
		{
			answer = Math::barryCentric(
				glm::vec3(0, getHeight(gridX + 1, gridZ), 0),
				glm::vec3(1, getHeight(gridX + 1, gridZ + 1), 0),
				glm::vec3(0, getHeight(gridX, gridZ + 1), 1),
				glm::vec2(xCoord, zCoord)
			);
		}
		return answer;
	}

	float getHeight(int x, int z)
	{
		return heights.at(z*VERTEX_COUNT + x);
	}
};
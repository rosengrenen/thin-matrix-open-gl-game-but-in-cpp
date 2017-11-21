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
	const int VERTEX_COUNT = 128;
public:
	float x;
	float z;
	Model model;
	TerrainTexturePack texturePack;
	TerrainTexture blendMap;
private:
	Model generateTerrain(Loader loader)
	{
		double startTime = glfwGetTime();

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
				vertices[vertexPointer * 3 + 1] = 0;
				vertices[vertexPointer * 3 + 2] = (float) i / ((float) VERTEX_COUNT - 1) * SIZE;

				normals[vertexPointer * 3] = 0;
				normals[vertexPointer * 3 + 1] = 1;
				normals[vertexPointer * 3 + 2] = 0;

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


		std::cout << "Time taken: " << glfwGetTime() - startTime << std::endl
			<< "Normals:  " << normals.size() << std::endl
			<< "Indices:  " << indices.size() << std::endl
			<< "Vertices: " << vertices.size() << std::endl
			<< "Textures: " << texCoords.size() << std::endl;

		return loader.loadToVao(vertices, texCoords, normals, indices);
	}
public:
	Terrain(int gridX, int gridZ, const Loader& loader, const TerrainTexturePack& texturePack, const TerrainTexture& blendMap) : x(gridX * SIZE), z(gridZ * SIZE), model(generateTerrain(loader)), texturePack(texturePack), blendMap(blendMap)
	{ }

	glm::mat4 getModelMatrix() const
	{
		return glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
	}
};
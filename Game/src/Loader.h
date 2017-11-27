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

	static Texture loadTexture2D(const std::string& path)
	{
		GLuint id;
		TextureData data(path);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(
			GL_TEXTURE_2D, // Target/type of texture
			0, // Level, always 0
			GL_RGBA, // Format to store the texture in, always store with Alpha channel
			data.getWidth(), // Width of texture
			data.getHeight(), // Height of texture
			0, // Border, always 0
			GL_RGBA, // Channels of input texture
			GL_UNSIGNED_BYTE, // Data type of input texture
			data.getData() // Texture data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		return Texture(id);
	}

	static Texture loadCubeMap(std::vector<std::string> paths)
	{
		GLuint id;
		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		for (int i = 0; i < paths.size(); i++)
		{
			//TODO: rename Image to TextureData and make it only hold the data, not a texture buffer
			TextureData texture(paths[i]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture.getWidth(), texture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getData());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//TODO: Move stuff from texture to Loader class, but make the constructor of Texture Take what requires for a cube map
		return Texture(id);
	}

	/* LOAD TO RAW MODEL */
	
	static RawModel loadObj(const std::string& path)
	{
		std::ifstream stream("res/objects/" + path + ".obj");
		std::string line;
		std::vector<float> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<int> indices;
		std::vector<float> outTexCoords;
		std::vector<float> outNormals;

		bool first = true;

		while (getline(stream, line))
		{
			std::vector<std::string> currentLine { Utilities::stringExplode(line, ' ') };
			if (line.substr(0, 2) == "v ")
			{
				vertices.emplace_back(::atof(currentLine[1].c_str()));
				vertices.emplace_back(::atof(currentLine[2].c_str()));
				vertices.emplace_back(::atof(currentLine[3].c_str()));
			}
			else if (line.substr(0, 3) == "vt ")
			{
				texCoords.emplace_back(glm::vec2(::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str())));
			}
			else if (line.substr(0, 3) == "vn ")
			{
				normals.emplace_back(glm::vec3(::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str()), ::atof(currentLine[3].c_str())));
			}
			else if (line.substr(0, 2) == "f ")
			{
				if (first)
				{
					first = false;
					outTexCoords.resize(vertices.size() * 2 / 3);
					outNormals.resize(vertices.size());
				}
				std::vector<std::string> vertex1 = Utilities::stringExplode(currentLine[1], '/');
				std::vector<std::string> vertex2 = Utilities::stringExplode(currentLine[2], '/');
				std::vector<std::string> vertex3 = Utilities::stringExplode(currentLine[3], '/');
				processVertex(vertex1, indices, texCoords, normals, outTexCoords, outNormals);
				processVertex(vertex2, indices, texCoords, normals, outTexCoords, outNormals);
				processVertex(vertex3, indices, texCoords, normals, outTexCoords, outNormals);
			}
		}

		return RawModel(vertices, outTexCoords, outNormals, indices);
	}

	static void processVertex(
		const std::vector<std::string>& vertexData, std::vector<int>& indices, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, std::vector<float>& outTexCoords, std::vector<float>& outNormals)
	{
		int currentVertexPointer = ::atoi(vertexData[0].c_str()) - 1;
		indices.push_back(currentVertexPointer);

		glm::vec2 currentTex = texCoords.at(::atoi(vertexData[1].c_str()) - 1);
		outTexCoords.at(currentVertexPointer * 2) = currentTex.x;
		outTexCoords.at(currentVertexPointer * 2 + 1) = 1 - currentTex.y;

		glm::vec3 currentNorm = normals.at(::atoi(vertexData[2].c_str()) - 1);
		outNormals.at(currentVertexPointer * 3) = currentNorm.x;
		outNormals.at(currentVertexPointer * 3 + 1) = currentNorm.y;
		outNormals.at(currentVertexPointer * 3 + 2) = currentNorm.z;
	}

};
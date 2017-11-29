#include "Loader.h"

/* LOAD TO TEXTURE */

void Loader::calculateTangents(VertexNM & v0, VertexNM & v1, VertexNM & v2, std::vector<glm::vec2>& textures)
{
	// The references do work, tangents are added
	glm::vec3 delatPos1 = v1.m_position - v0.m_position;
	glm::vec3 delatPos2 = v2.m_position - v0.m_position;
	glm::vec2 uv0 = textures.at(v0.m_textureIndex);
	glm::vec2 uv1 = textures.at(v1.m_textureIndex);
	glm::vec2 uv2 = textures.at(v2.m_textureIndex);
	glm::vec2 deltaUv1 = uv1 - uv0;
	glm::vec2 deltaUv2 = uv2 - uv0;

	float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);
	delatPos1 *= deltaUv2.y;
	delatPos2 *= deltaUv1.y;
	glm::vec3 tangent = delatPos1 - delatPos2;
	tangent *= r;
	v0.addTangent(tangent);
	v1.addTangent(tangent);
	v2.addTangent(tangent);
}

VertexNM& Loader::processVertex(std::vector<std::string>& vertex, std::vector<VertexNM>& vertices, std::vector<int>& indices)
{
	int index = ::atoi(vertex[0].c_str()) - 1;
	VertexNM& currentVertex = vertices.at(index);
	int textureIndex = ::atoi(vertex[1].c_str()) - 1;
	int normalIndex = ::atoi(vertex[2].c_str()) - 1;
	if (!currentVertex.isSet())
	{
		currentVertex.m_textureIndex = textureIndex;
		currentVertex.m_normalIndex = normalIndex;
		indices.push_back(index);
		return currentVertex;
	}
	else
	{
		//return dealWithAlreadyProcessedVertex(currentVertex, textureIndex, normalIndex, indices,vertices);
		if (currentVertex.hasSameTextureAndNormal(textureIndex, normalIndex))
		{
			indices.push_back(currentVertex.m_index);
			return currentVertex;
		}
		else
		{
			VertexNM* anotherVertex = currentVertex.m_duplicateVertex;
			if (anotherVertex)
			{
				return dealWithAlreadyProcessedVertex(*anotherVertex, textureIndex,
					normalIndex, indices, vertices);
			}
			else
			{
				VertexNM* duplicateVertex = new VertexNM(currentVertex.duplicate(vertices.size()));//NEW
				duplicateVertex->m_textureIndex = textureIndex;
				duplicateVertex->m_normalIndex = normalIndex;
				currentVertex.setDuplicateVertex(duplicateVertex);
				vertices.push_back(*duplicateVertex);
				indices.push_back(duplicateVertex->m_index);
				return *duplicateVertex;
			}
		}
	}
}

float Loader::convertDataToArrays(std::vector<VertexNM>& vertices, std::vector<glm::vec2>& textures, std::vector<glm::vec3>& normals, std::vector<float>& verticesArray, std::vector<float>& texturesArray, std::vector<float>& normalsArray, std::vector<float>& tangentsArray)
{
	float furthestPoint = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		VertexNM currentVertex = vertices.at(i);
		if (currentVertex.m_length > furthestPoint)
		{
			furthestPoint = currentVertex.m_length;
		}
		glm::vec3 position = currentVertex.m_position;
		glm::vec2 textureCoord = textures.at(currentVertex.m_textureIndex);
		glm::vec3 normalVector = normals.at(currentVertex.m_normalIndex);
		glm::vec3 tangent = currentVertex.m_averagedTangent;
		verticesArray.at(i * 3) = position.x;
		verticesArray.at(i * 3 + 1) = position.y;
		verticesArray.at(i * 3 + 2) = position.z;
		texturesArray.at(i * 2) = textureCoord.x;
		texturesArray.at(i * 2 + 1) = 1 - textureCoord.y;
		normalsArray.at(i * 3) = normalVector.x;
		normalsArray.at(i * 3 + 1) = normalVector.y;
		normalsArray.at(i * 3 + 2) = normalVector.z;
		tangentsArray.at(i * 3) = tangent.x;
		tangentsArray.at(i * 3 + 1) = tangent.y;
		tangentsArray.at(i * 3 + 2) = tangent.z;

	}
	return furthestPoint;
}

VertexNM& Loader::dealWithAlreadyProcessedVertex(VertexNM& previousVertex, int newTextureIndex, int newNormalIndex, std::vector<int>& indices, std::vector<VertexNM>& vertices)
{
	if (previousVertex.hasSameTextureAndNormal(newTextureIndex, newNormalIndex))
	{
		indices.push_back(previousVertex.m_index);
		return previousVertex;
	}
	else
	{
		VertexNM* anotherVertex = previousVertex.m_duplicateVertex;
		if (anotherVertex)
		{
			return dealWithAlreadyProcessedVertex(*anotherVertex, newTextureIndex,
				newNormalIndex, indices, vertices);
		}
		else
		{
			VertexNM* duplicateVertex = new VertexNM(previousVertex.duplicate(vertices.size()));//NEW
			duplicateVertex->m_textureIndex = newTextureIndex;
			duplicateVertex->m_normalIndex = newNormalIndex;
			previousVertex.setDuplicateVertex(duplicateVertex);
			vertices.push_back(*duplicateVertex);
			indices.push_back(duplicateVertex->m_index);
			return *duplicateVertex;
		}
	}
}

void Loader::removeUnusedVertices(std::vector<VertexNM>& vertices)
{
	for (VertexNM& vertex : vertices)
	{
		vertex.averageTangents();
		if (!vertex.isSet())
		{
			vertex.m_textureIndex = 0;
			vertex.m_normalIndex = 0;
		}
	}
}

Texture Loader::loadTexture2D(const std::string& path)
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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -2.5f);
	return Texture(id);
}

Texture Loader::loadCubeMap(std::vector<std::string> paths)
{
	GLuint id;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (int i = 0; i < paths.size(); i++)
	{
		TextureData texture(paths[i]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture.getWidth(), texture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getData());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return Texture(id);
}

/* LOAD TO RAW MODEL */

RawModel Loader::loadObj(const std::string& path)
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
			vertices.push_back(::atof(currentLine[1].c_str()));
			vertices.push_back(::atof(currentLine[2].c_str()));
			vertices.push_back(::atof(currentLine[3].c_str()));
		}
		else if (line.substr(0, 3) == "vt ")
		{
			texCoords.push_back({ ::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str()) });
		}
		else if (line.substr(0, 3) == "vn ")
		{
			normals.push_back({ ::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str()), ::atof(currentLine[3].c_str()) });
		}
		else if (line.substr(0, 2) == "f ")
		{
			if (first)
			{
				first = false;
				outTexCoords.resize(vertices.size() * 2 / 3);
				outNormals.resize(vertices.size());
			}
			for (int i = 0; i < 3; i++)
			{
				std::vector<std::string> v = Utilities::stringExplode(currentLine[i + 1], '/');
				int currentVertexPointer = ::atoi(v[0].c_str()) - 1;
				indices.push_back(currentVertexPointer);

				glm::vec2 currentTex = texCoords.at(::atoi(v[1].c_str()) - 1);
				outTexCoords.at(currentVertexPointer * 2) = currentTex.x;
				outTexCoords.at(currentVertexPointer * 2 + 1) = 1 - currentTex.y;

				glm::vec3 currentNorm = normals.at(::atoi(v[2].c_str()) - 1);
				outNormals.at(currentVertexPointer * 3) = currentNorm.x;
				outNormals.at(currentVertexPointer * 3 + 1) = currentNorm.y;
				outNormals.at(currentVertexPointer * 3 + 2) = currentNorm.z;
			}
		}
	}
	return RawModel(vertices, outTexCoords, outNormals, indices);
}

RawModel Loader::loadNormalObj(const std::string & path)
{
	std::ifstream stream("res/objects/" + path + ".obj");
	std::string line;
	std::vector<VertexNM> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;
	while (getline(stream, line))
	{
		std::vector<std::string> currentLine { Utilities::stringExplode(line, ' ') };
		if (line.substr(0, 2) == "v ")
		{
			glm::vec3 vertex(::atof(currentLine[1].c_str()),
				::atof(currentLine[2].c_str()),
				::atof(currentLine[3].c_str()));
			VertexNM newVertex(vertices.size(), vertex);
			vertices.push_back(newVertex);

		}
		else if (line.substr(0, 3) == "vt ")
		{
			glm::vec2 texture(::atof(currentLine[1].c_str()),
				::atof(currentLine[2].c_str()));
			textures.push_back(texture);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			glm::vec3 normal(::atof(currentLine[1].c_str()),
				::atof(currentLine[2].c_str()),
				::atof(currentLine[3].c_str()));
			normals.push_back(normal);
		}
		else if (line.substr(0, 2) == "f ")
		{
			break;
		}
	}
	do
	{
		if (line.substr(0, 2) != "f ")
			break;
		std::vector<std::string> currentLine = Utilities::stringExplode(line, ' ');
		std::vector<std::string> vertex1 = Utilities::stringExplode(currentLine[1], '/');
		std::vector<std::string> vertex2 = Utilities::stringExplode(currentLine[2], '/');
		std::vector<std::string> vertex3 = Utilities::stringExplode(currentLine[3], '/');
		VertexNM& v0 = processVertex(vertex1, vertices, indices);
		VertexNM& v1 = processVertex(vertex2, vertices, indices);
		VertexNM& v2 = processVertex(vertex3, vertices, indices);
		calculateTangents(v0, v1, v2, textures);//NEW
	} while (getline(stream, line));
	removeUnusedVertices(vertices);
	std::vector<float> verticesArray(vertices.size() * 3);
	std::vector<float> texturesArray(vertices.size() * 2);
	std::vector<float> normalsArray(vertices.size() * 3);
	std::vector<float> tangentsArray(vertices.size() * 3);
	float furthest = convertDataToArrays(vertices, textures, normals, verticesArray,
		texturesArray, normalsArray, tangentsArray);

	return { verticesArray, texturesArray, normalsArray, tangentsArray, indices };
}

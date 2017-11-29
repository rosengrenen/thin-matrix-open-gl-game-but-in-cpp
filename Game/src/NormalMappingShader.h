#pragma once

#include <glm\mat4x4.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

#include "ShaderProgram.h"
#include "Light.h"

#include <vector>

class NormalMappingShader : public ShaderProgram
{
private:
	const int MAX_LIGHTS = 4;

	const std::string PATH = "res/shaders/normalMap.shader";

	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	std::vector<int> location_lightPosition;
	std::vector<int> location_lightColour;
	std::vector<int> location_attenuation;
	int location_shineDamper;
	int location_reflectivity;
	int location_skyColour;
	int location_numberOfRows;
	int location_offset;
	int location_plane;
	int location_modelTexture;
	int location_normalMap;
public:
	NormalMappingShader()
	{
		ShaderProgram::createShaderProgram(PATH);
	}
private:
	void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
		bindAttribLocation(1, "textureCoordinates");
		bindAttribLocation(2, "normal");
		bindAttribLocation(3, "tangent");
	}

	void getUniformLocations() override
	{
		location_transformationMatrix = getUniformLocation("model");
		location_projectionMatrix = getUniformLocation("projection");
		location_viewMatrix = getUniformLocation("view");
		location_shineDamper = getUniformLocation("shineDamper");
		location_reflectivity = getUniformLocation("reflectivity");
		location_skyColour = getUniformLocation("skyColour");
		location_numberOfRows = getUniformLocation("numberOfRows");
		location_offset = getUniformLocation("offset");
		location_plane = getUniformLocation("plane");
		location_modelTexture = getUniformLocation("modelTexture");
		setInt(location_modelTexture, 0);
		location_normalMap = getUniformLocation("normalMap");
		setInt(location_normalMap, 1);
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			location_lightPosition.push_back(getUniformLocation("lightPosition[" + std::to_string(i) + "]"));
			location_lightColour.push_back(getUniformLocation("lightColour[" + std::to_string(i) + "]"));
			location_attenuation.push_back(getUniformLocation("attenuation[" + std::to_string(i) + "]"));
		}
	}
public:
	void setClipPlane(glm::vec4 plane) const
	{
		setVector4f(location_plane, plane);
	}

	void setNumberOfRows(int numberOfRows) const
	{
		setFloat(location_numberOfRows, numberOfRows);
	}

	void setOffset(float x, float y) const
	{
		setVector2f(location_offset, { x, y });
	}

	void setSkyColour(float r, float g, float b) const
	{
		setVector3f(location_skyColour, { r, g, b });
	}

	void setShineVariables(float damper, float reflectivity) const
	{
		setFloat(location_shineDamper, damper);
		setFloat(location_reflectivity, reflectivity);
	}

	void setModelMatrix(const glm::mat4& matrix) const
	{
		setMatrix4f(location_transformationMatrix, matrix);
	}

	void setLights(const std::vector<Light>& lights) const
	{
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			if (i < lights.size())
			{
				setVector3f(location_lightPosition[i], lights.at(i).position);
				setVector3f(location_lightColour[i], lights.at(i).colour);
				setVector3f(location_attenuation[i], lights.at(i).attenuation);
			}
			else
			{
				setVector3f(location_lightPosition[i], { 0, 0, 0 });
				setVector3f(location_lightColour[i], { 0, 0, 0 });
				setVector3f(location_attenuation[i], { 1, 0, 0 });
			}
		}
	}

	void setViewMatrix(const glm::mat4& viewMatrix) const
	{
		setMatrix4f(location_viewMatrix, viewMatrix);
	}

	void setProjectionMatrix(const glm::mat4& projection) const
	{
		setMatrix4f(location_projectionMatrix, projection);
	}
};
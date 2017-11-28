#pragma once

#include "ShaderProgram.h"

class WaterShader : public ShaderProgram
{
private:
	const std::string path = "res/shaders/water.shader";

	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
	int m_reflectionLoc;
	int m_refractionLoc;
public:
	WaterShader()
	{
		ShaderProgram::createShaderProgram(path);
	}

	void bindAttribLocations() override
	{
		bindAttribLocation(0, "position");
	}

	void getUniformLocations() override
	{
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
		location_modelMatrix = getUniformLocation("modelMatrix");
		m_reflectionLoc = getUniformLocation("reflection");
		m_refractionLoc = getUniformLocation("refraction");
		setInt(m_reflectionLoc, 0);
		setInt(m_refractionLoc, 1);
	}

	void setProjectionMatrix(const Camera& camera)
	{
		setMatrix4f(location_projectionMatrix, camera.getProjectionMatrix());
	}

	void setViewMatrix(const Camera& camera)
	{
		setMatrix4f(location_viewMatrix, camera.getViewMatrix());
	}

	void setModelMatrix(const glm::mat4& modelMatrix)
	{
		setMatrix4f(location_modelMatrix, modelMatrix);
	}

};
#pragma once

#include "ShaderProgram.h"

class WaterShader : public ShaderProgram
{
private:
	const std::string path = "res/shaders/water.shader";

	int location_modelMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
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
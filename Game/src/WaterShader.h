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
	int m_dudvMapLoc;
	int m_moveFactorLoc;
	int m_camLoc;
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
		m_moveFactorLoc = getUniformLocation("moveFactor");
		m_dudvMapLoc = getUniformLocation("dudvMap");
		m_camLoc = getUniformLocation("cameraPosition");
		setInt(m_reflectionLoc, 0);
		setInt(m_refractionLoc, 1);
		setInt(m_dudvMapLoc, 2);
	}

	void setProjectionMatrix(const Camera& camera)
	{
		setMatrix4f(location_projectionMatrix, camera.getProjectionMatrix());
	}

	void setViewMatrix(Camera& camera)
	{
		setMatrix4f(location_viewMatrix, camera.getViewMatrix());
		setVector3f(m_camLoc, camera.getPosition());
	}

	void setModelMatrix(const glm::mat4& modelMatrix)
	{
		setMatrix4f(location_modelMatrix, modelMatrix);
	}

	void setMoveFactor(float factor)
	{
		setFloat(m_moveFactorLoc, factor);
	}
};
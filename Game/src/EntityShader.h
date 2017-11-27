#pragma once

#include <glm\mat4x4.hpp>
#include <vector>

#include "Light.h"
#include "ShaderProgram.h"

class EntityShader : public ShaderProgram
{
private:
	const int MAX_LIGHTS = 4;
	const std::string m_shaderPath = "res/shaders/entity.shader";
	unsigned int m_modelMatrixLoc;
	unsigned int m_viewMatrixLoc;
	unsigned int m_projectionMatrixLoc;
	std::vector<unsigned int> m_lightPositionLoc;
	std::vector<unsigned int> m_lightColourLoc;
	std::vector<unsigned int> m_lightAttenuationLoc;
	unsigned int m_reflectivityLoc;
	unsigned int m_shineDamperLoc;
	unsigned int m_fakeNormalsLoc;
	unsigned int m_skyColourLoc;
	unsigned int m_atlasOffsetLoc;
	unsigned int m_numRowsLoc;
	unsigned int m_planeLoc;
public:
	EntityShader();

	virtual void bindAttribLocations() override;

	virtual void getUniformLocations() override;

	void setModelMatrix(const glm::mat4& matrix) const;

	void setProjectionMatrix(const glm::mat4& matrix) const;

	void setViewMatrix(const glm::mat4& matrix) const;

	void setLight(const std::vector<Light>& lights) const;

	void setShineVariables(float reflectivity, float damper);

	void setFakeLighting(bool useFake);

	void setSkyColour(float r, float g, float b);

	void setNumRows(int numRows);

	void setAtlasOffset(float x, float y);

	void setClipPlane(const glm::vec4& plane);
};
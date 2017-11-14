#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include "ShaderProgram.h"

#include "..\entities\Camera.h"
#include "..\utilities\Maths.h"

class StaticShader : public ShaderProgram
{
private:
	int m_transformationMatrixLocation;
	int m_projectionMatrixLocation;
	int m_viewMatrixLocation;
public:
	StaticShader(const std::string& filePath)
	{
		createProgram(filePath);
	}
	void getAllUniformLocations() override
	{
		m_transformationMatrixLocation = getUniformLocation("transformationMatrix");
		m_projectionMatrixLocation = getUniformLocation("projectionMatrix");
		m_viewMatrixLocation = getUniformLocation("viewMatrix");
	}
	void bindAttributes() override
	{
		bindAttribute(0, "position");
		bindAttribute(1, "texCoords");
	}
	void loadTransformationMatrix(glm::mat4 matrix)
	{
		setMatrix4(m_transformationMatrixLocation, matrix);
	}

	void loadViewMatrix(Camera camera)
	{
		glm::mat4 view = camera.getViewMatrix();
		setMatrix4(m_viewMatrixLocation, view);
	}

	void loadProjectionMatrix(glm::mat4 matrix)
	{
		setMatrix4(m_projectionMatrixLocation, matrix);
	}
};

#endif
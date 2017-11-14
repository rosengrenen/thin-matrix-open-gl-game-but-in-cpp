#ifndef RENDERER_H
#define RENDERER_H

#include <GL\glew.h>

#include "..\entities\Entity.h"
#include "..\models\RawModel.h"
#include "..\shaders\StaticShader.h"
#include "..\utilities\Maths.h"

class Renderer
{
private:
	const float m_FOV = 70;
	const float m_NEAR_PLANE = 0.1f;
	const float m_FAR_PLANE = 1000;
	glm::mat4 m_projectionMatrix;
private:
	void createProjectMatrix()
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_FOV), 800.0f / 600.0f, m_NEAR_PLANE, m_FAR_PLANE);
	}
public:
	Renderer(StaticShader& shader)
	{
		createProjectMatrix();
		shader.use();
		shader.loadProjectionMatrix(m_projectionMatrix);
		shader.stop();
	}
	void prepare()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	}

	void render(Entity entity, StaticShader& shader)
	{
		TexturedModel model = entity.getModel();
		RawModel rawModel = model.getRawModel();
		glBindVertexArray(rawModel.getVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glm::mat4 transMat = Maths::createTransformationMatrix(entity.getPosition(), entity.getRotation(), entity.getScale());
		shader.loadTransformationMatrix(transMat);
		int test = model.getTexture().getID();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.getTexture().getID());
		glDrawElements(GL_TRIANGLES, rawModel.getVertexCount(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
};

#endif
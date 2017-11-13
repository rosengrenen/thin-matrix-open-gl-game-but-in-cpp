#ifndef RENDERER_H
#define RENDERER_H

#include <GL\glew.h>

#include "..\entities\Entity.h"
#include "..\models\RawModel.h"
#include "..\shaders\StaticShader.h"
#include "..\utilities\Maths.h"

class Renderer
{
public:
	void prepare()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
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
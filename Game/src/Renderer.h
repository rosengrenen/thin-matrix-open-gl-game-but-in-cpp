#pragma once

class Renderer
{
public:
	Renderer()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	void prepare()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
	}

	void render(Entity entity)
	{
		entity.model.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		entity.texture.bind();

		glDrawElements(GL_TRIANGLES, entity.model.numVertices, GL_UNSIGNED_INT, nullptr);

		entity.texture.unbind();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		entity.model.unbind();
	}

	void stop()
	{
		glDisable(GL_DEPTH_TEST);
	}
};
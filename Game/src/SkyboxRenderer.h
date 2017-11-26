#pragma once

#include <vector>
#include "Model.h"
#include "SkyboxShader.h"
#include <GL\glew.h>
#include "Loader.h"
#include "Camera.h"

class SkyboxRenderer
{
private:
	const float SIZE = 500.0f;
	const std::vector<float> VERTICES {
			-SIZE, SIZE, -SIZE,
			-SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE, SIZE, -SIZE,
			-SIZE, SIZE, -SIZE,

			-SIZE, -SIZE, SIZE,
			-SIZE, -SIZE, -SIZE,
			-SIZE, SIZE, -SIZE,
			-SIZE, SIZE, -SIZE,
			-SIZE, SIZE, SIZE,
			-SIZE, -SIZE, SIZE,

			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, SIZE,
			SIZE, SIZE, SIZE,
			SIZE, SIZE, SIZE,
			SIZE, SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,

			-SIZE, -SIZE, SIZE,
			-SIZE, SIZE, SIZE,
			SIZE, SIZE, SIZE,
			SIZE, SIZE, SIZE,
			SIZE, -SIZE, SIZE,
			-SIZE, -SIZE, SIZE,

			-SIZE, SIZE, -SIZE,
			SIZE, SIZE, -SIZE,
			SIZE, SIZE, SIZE,
			SIZE, SIZE, SIZE,
			-SIZE, SIZE, SIZE,
			-SIZE, SIZE, -SIZE,

			-SIZE, -SIZE, -SIZE,
			-SIZE, -SIZE, SIZE,
			SIZE, -SIZE, -SIZE,
			SIZE, -SIZE, -SIZE,
			-SIZE, -SIZE, SIZE,
			SIZE, -SIZE, SIZE
	};
public:
	std::vector<std::string> TEXTURE_FILES {
		"right.png",
		"left.png",
		"top.png",
		"bottom.png",
		"back.png",
		"front.png"
	};
	Model cube;
	GLint texture;
	SkyboxShader shader;
public:
	SkyboxRenderer() : cube(Loader::loadToVao(VERTICES, 3)), texture(Loader::loadCubeMap(TEXTURE_FILES))
	{ 

	}

	void render(Camera camera)
	{
		shader.use();
		shader.setViewMatrix(camera.getViewMatrix());
		shader.setProjectionMatrix(camera.getProjectionMatrix(1.33333f));
		glBindVertexArray(cube.getID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, cube.numVertices);
		glDisableVertexAttribArray(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindVertexArray(0);
	}


};
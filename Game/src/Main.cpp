#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\constants.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb_image.h"

#include "Camera.h"
#include "Entity.h"
#include "Renderer.h"
#include "StaticShader.h"
#include "Window.h"

#pragma region GL_DEBUG_TOOLS
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}
#pragma endregion

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* Create a windowed mode window and its OpenGL context 
	 * Keyboard and Mouse classes should be separate to the Window class, they'll just take the Window pointer as a constructor argument
	 */
	Window mWindow(800, 600, "New window");

	// Temporary solution to get rid of errors, need more abstractions
	GLFWwindow* window = mWindow.getWindow();

	#pragma region GLEW_INIT
	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	#pragma endregion

	#pragma region VERTEX DATA
	std::vector<float> vertices {
		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,0.5f,-0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,-0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,

		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,

		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f
	};
	std::vector<float> texCoords {
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0,
		0,0,
		0,1,
		1,1,
		1,0
	};
	std::vector<int> indices {
		0,1,3,
		3,1,2,
		4,5,7,
		7,5,6,
		8,9,11,
		11,9,10,
		12,13,15,
		15,13,14,
		16,17,19,
		19,17,18,
		20,21,23,
		23,21,22
	};
	#pragma endregion

	// How to abstract this?? VAO -> VBOS (VERTICES, TEXTURES) -> TRANSFORMATION -> Entity
	#pragma region DATA
	unsigned int vao;
	unsigned int vbos[2];
	unsigned int ibo;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW));

	GLCall(glGenBuffers(2, vbos));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbos[0]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbos[1]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords.front(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	#pragma endregion Generates the VAO, array buffers and index buffer

	#pragma region TEXTURE
	unsigned int texture;
	GLCall(glGenTextures(1, &texture));
	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Couldn't load texture" << std::endl;
	}
	#pragma endregion Loads and binds a texture (container.jpg)

	StaticShader shader("res/shaders/triangle.shader");

	Model cubeModel(vertices, texCoords, indices);

	Texture cubeTexture("res/textures/container.jpg");

	Entity cube(
		cubeModel,
		cubeTexture,
		glm::vec3(2, 0, 2),
		glm::vec3(0, 0, 0),
		0.8f
	);

	Camera camera(glm::vec3(0));

	Renderer renderer;

	// !! WINDOW <- SCRAP THAT -> FPS COUNTER CLASS?
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;

	double mouseSensitivity = 0.3;

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float movementSpeed = 0.1f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Method naming should be changed...
		mWindow.prepare();

		cube.rotate(0, 0.005f, 0);

		/* Rotate camera from mouse input */
		camera.rotate(mWindow.mouseOffsetY() * mouseSensitivity, mWindow.mouseOffsetX() * mouseSensitivity);

		/* Mouse the camera from keyboard input */
		if (mWindow.isKeyPressed(GLFW_KEY_W))
		{
			camera.moveFront(movementSpeed, 0, movementSpeed);
		}
		else if (mWindow.isKeyPressed(GLFW_KEY_S))
		{
			camera.moveFront(-movementSpeed, 0, -movementSpeed);
		}
		if (mWindow.isKeyPressed(GLFW_KEY_A))
		{
			camera.moveRight(movementSpeed, 0, movementSpeed);
		}
		else if (mWindow.isKeyPressed(GLFW_KEY_D))
		{
			camera.moveRight(-movementSpeed, 0, -movementSpeed);
		}
		if (mWindow.isKeyPressed(GLFW_KEY_SPACE))
		{
			camera.move(0, -movementSpeed, 0);
		}
		else if (mWindow.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			camera.move(0, movementSpeed, 0);
		}

		// Render here !! WHERE DOES THIS BELONG? PREPARATION !! NOT IMPORTANT FOR NOW
		
		renderer.prepare();

		shader.use();
		shader.setTransformationMatrix(cube.getTransformationMatrix());
		shader.setProjectionMatrix(camera.getProjectionMatrix(mWindow.getAspectRatio()));
		shader.setViewMatrix(camera.getViewMatrix());

		// ENTITY STUFF (COORDS)
		GLCall(glBindVertexArray(vao));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));

		// ENTITY TEX
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));

		// RENDERING
		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glDisableVertexAttribArray(0));
		GLCall(glDisableVertexAttribArray(1));
		GLCall(glBindVertexArray(0));

		GLCall(glUseProgram(0));

		//GLCall(glDisable(GL_DEPTH_TEST));

		mWindow.swapBuffers();

		/* Poll for and process events !! WINDOW?? */
		GLCall(glfwPollEvents());
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// FPS !! WINDOW? !! FPS?
		frames++;
		if (currentFrame - frameTime > 1.0f)
		{
			std::cout << "[FPS] " << frames << std::endl;
			frameTime = currentFrame;
			frames = 0;
		}
	}

	glfwTerminate();
	return 0;
}
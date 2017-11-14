#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>

#include "entities\Entity.h"
#include "entities\Camera.h"
#include "models\RawModel.h"
#include "models\TexturedModel.h"
#include "textures\ModelTexture.h"
#include "shaders/StaticShader.h"
#include "renderEngine/DisplayManager.h"
#include "renderEngine/Loader.h"
#include "renderEngine/Renderer.h"
#include "utilities\Maths.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb_image.h"

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

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(800, 600, "De-abstraction of everything", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//Loader loader;
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

	// Generate cube vao and pass data
	unsigned int vao;
	unsigned int vbos[2];
	unsigned int ibo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(2, vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.begin(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords.begin(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Bind a texture to the cube
	unsigned int texture;
	glGenTextures(1, &texture);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/textures/container.jps", &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Generate shader program
	unsigned int program;

	/*RawModel model = loader.loadToVAO(vertexes, texCoords, indices);
	//ModelTexture texture(loader.loadTexture("wall.jpg"));
	//TexturedModel staticModel(model, texture);
	StaticShader shader("res/shaders/triangle.shader");
	//Renderer renderer(shader);
	Entity entity(staticModel, glm::vec3(0), glm::vec3(0), 0.8f);

	Camera camera;*/

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//camera.processInput(gWindow, deltaTime);
		//entity.rotate(glm::vec3(0, 1, 0));
		/* Render here */
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*renderer.prepare();
		GLCall(shader.use());
		shader.loadViewMatrix(camera);
		GLCall(renderer.render(entity, shader));
		GLCall(shader.stop());*/

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	glfwTerminate();
	return 0;
}
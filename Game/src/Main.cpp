#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb_image.h"

#include "ShaderProgram.h"

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
		return 0;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(800, 600, "De-abstraction of everything", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 0;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//Loader loader;
	/*std::vector<float> vertices {
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
	};*/

	std::vector<float> vertices = {
		0
	};

	// Generate cube vao and pass data
	unsigned int vao;
	unsigned int vbos[2];
	unsigned int ibo;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	GLCall(glGenBuffers(2, vbos));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbos[0]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbos[1]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords.front(), GL_STATIC_DRAW));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0));

	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW));

	// Bind a texture to the cube
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

	// Generate shader program
	unsigned int program = glCreateProgram();

	std::tuple<std::string, std::string> shaderSource = parseShaderSource("res/shaders/triangle.shader");
	const char* vertexSource = std::get<0>(shaderSource).c_str();
	const char* fragmentSource = std::get<1>(shaderSource).c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertexShader, 1, &vertexSource, nullptr));
	GLCall(glCompileShader(vertexShader));
	int result;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(vertexShader, length, &length, message);
		std::cout << "Failed to compile vertex shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(vertexShader);
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, nullptr));
	GLCall(glCompileShader(fragmentShader));

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(fragmentShader, length, &length, message);
		std::cout << "Failed to compile fragment shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(fragmentShader);
	}

	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));

	GLCall(glBindAttribLocation(program, 0, "position"));
	GLCall(glBindAttribLocation(program, 1, "texCoords"));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//unsigned int transformationMatrixLoc = glGetUniformLocation(program, "transformationMatrix");
	//unsigned int projectionMatrixLoc = glGetUniformLocation(program, "projectionMatrix");
	//unsigned int viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");
	/*RawModel model = loader.loadToVAO(vertexes, texCoords, indices);
	//ModelTexture texture(loader.loadTexture("wall.jpg"));
	//TexturedModel staticModel(model, texture);
	StaticShader shader("res/shaders/trianGLCall(gle.shader"));
	//Renderer renderer(shader);
	Entity entity(staticModel, GLCall(glm::vec3(0), glm::vec3(0), 0.8f));

	Camera camera;*/

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	//GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glUseProgram(program));

		GLCall(glBindVertexArray(vao));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));

		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices.front()));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glDisableVertexAttribArray(0));
		GLCall(glDisableVertexAttribArray(1));
		GLCall(glBindVertexArray(0));

		GLCall(glUseProgram(0));

		GLCall(glDisable(GL_DEPTH_TEST));

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GLCall(glfwPollEvents());
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	glfwTerminate();
	return 0;
}
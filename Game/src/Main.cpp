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

#include "ShaderProgram.h"
#include "Window.h"

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

// The object in question
static glm::mat4 getTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotation, const float scale)
{
	// Translate
	glm::mat4 transformationMatrix = glm::translate(glm::mat4(1), translation);
	// RotateX
	transformationMatrix = glm::rotate(transformationMatrix, rotation.x, glm::vec3(1, 0, 0));
	// RotateY
	transformationMatrix = glm::rotate(transformationMatrix, rotation.y, glm::vec3(0, 1, 0));
	// RotateZ
	transformationMatrix = glm::rotate(transformationMatrix, rotation.z, glm::vec3(0, 0, 1));
	// Scale
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(scale));
	return transformationMatrix;
}

// Camera
static glm::mat4 getViewMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	return glm::lookAt(eye, center, up);
}

// Related to the window and fov (camera)
static glm::mat4 getProjectionMatrix(const float fov)
{
	return glm::perspective(fov, 800.0f / 600.0f, 0.1f, 100.0f);
}

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return 0;

	/* Create a windowed mode window and its OpenGL context */
	Window mWindow(800, 600, "New window");

	// Temporary solution to get rid of errors, need more abstractions
	GLFWwindow* window = mWindow.getWindow();

	// Unlimit the framerate
	glfwSwapInterval(1);

	unsigned int err = glewInit();
	if (err != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

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

	// All of this in a separate file, make it a class or something // think of the abstraction and virtual functions
	#pragma region SHADER_PROGRAM
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

	unsigned int transformationMatrixLoc = glGetUniformLocation(program, "transformationMatrix");
	unsigned int projectionMatrixLoc = glGetUniformLocation(program, "projectionMatrix");
	unsigned int viewMatrixLoc = glGetUniformLocation(program, "viewMatrix");
	#pragma endregion Creates shader program, attaches shaders, links, and validates

	// Cube properties !! ENTITY
	glm::vec3 translation(-0.2f, 0.3f, -0.5);
	glm::vec3 rotation(0, -90, 0);
	float scale = 0.8f;

	// World properties !! WORLD
	glm::vec3 worldUp(0, 1, 0);

	// Camera properties !! CAMERA
	// Location of the camera
	glm::vec3 position(0, 0, 1);
	// Focus point
	glm::vec3 front;
	// Up direction of camera, wont be changed yet, tilting the camera isn't necessary yet
	glm::vec3 up;
	// Vector used to caluclate up
	glm::vec3 right;
	// Rotation around y axis
	float yaw = 0;
	// Rotation around x axis
	float pitch = 0;
	// Rotation around z axis
	float roll = 0;
	// Field of View
	float FoV = 45;

	// !! WINDOW
	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	int frames = 0;
	float frameTime = 0;

	double mouseX = 400;
	double mouseY = 300;
	double mouseOffsetX;
	double mouseOffsetY;
	double mouseLastX = 0;
	double mouseLastY = 0;
	double mouseSensitivity = 0.3;

	glfwSetCursorPos(window, mouseX, mouseY);

	GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));

	float movementSpeed = 0.2f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Change entity properites !! ENTITY
		rotation.y += 0.01f;

		// Mouse movement !! WINDOW !! CAMERA
		glfwGetCursorPos(window, &mouseX, &mouseY);
		mouseOffsetX = mouseLastX - mouseX;
		mouseOffsetY = mouseY - mouseLastY;
		yaw += mouseOffsetY * mouseSensitivity;
		yaw = yaw < -89 ? -89 : yaw;
		yaw = yaw > 89 ? 89 : yaw;
		pitch += mouseOffsetX * mouseSensitivity;
		mouseLastX = mouseX;
		mouseLastY = mouseY;
		front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
		front.y = glm::sin(glm::radians(yaw));
		front.z = glm::sin(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(front, right));

		// Keyboard input !! WINDOW
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += glm::vec3(front.x, 0, front.z) * movementSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position -= glm::vec3(front.x, 0, front.z) * movementSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += right * movementSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position -= right * movementSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			position.y -= movementSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			position.y += movementSpeed;
		}

		// Render here !! WHERE DOES THIS BELONG?
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glUseProgram(program));

		glUniformMatrix4fv(transformationMatrixLoc, 1, GL_FALSE, &getTransformationMatrix(translation, rotation, scale)[0][0]);
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &getViewMatrix(position, position + front, up)[0][0]);
		glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, &getProjectionMatrix(FoV)[0][0]);

		GLCall(glBindVertexArray(vao));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));

		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		GLCall(glDisableVertexAttribArray(0));
		GLCall(glDisableVertexAttribArray(1));
		GLCall(glBindVertexArray(0));

		GLCall(glUseProgram(0));

		GLCall(glDisable(GL_DEPTH_TEST));

		/* Swap front and back buffers !! WINDOW */
		GLCall(glfwSwapBuffers(window));

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
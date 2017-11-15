#ifndef CAMERA_H
#define CAMERA_H

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>

// Initial camera values
static const float YAW = -90.0f;
static const float PITCH = 0.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.3f;
static const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	float m_pitch;
	float m_yaw;
	//float m_roll;
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	float m_mouseLastX = 400;
	float m_mouseLastY = 300;
public:
	Camera(glm::vec3 position = glm::vec3(0),
		glm::vec3 up = glm::vec3(0, 1, 0),
		float yaw = YAW,
		float pitch = PITCH)
		: m_front(glm::vec3(0, 0, -1)),
		m_movementSpeed(SPEED),
		m_zoom(ZOOM),
		m_position(position),
		m_worldUp(up),
		m_yaw(yaw),
		m_pitch(pitch),
		m_mouseSensitivity(SENSITIVITY)
	{
		updateCameraVectors();
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void processInput(GLFWwindow* window, const float deltaTime)
	{
		processKeyboardInput(window, deltaTime);
		processMouseMovement(window, deltaTime);
		//processMouseScroll(window, deltaTime);

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	void processKeyboardInput(GLFWwindow* window, const float deltaTime)
	{
		float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_position += cameraSpeed * m_front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_position -= cameraSpeed * m_front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_position -= glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_position += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
	}

	void processMouseMovement(GLFWwindow* window, const float deltaTime, bool constrainPitch = true)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float xoffset = mouseX - m_mouseLastX;
		float yoffset = m_mouseLastY - mouseY; // reversed since y-coordinates range from bottom to top
		m_mouseLastX = mouseX;
		m_mouseLastY = mouseY;
		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;
		std::cout << "x: " << mouseX << " y: " << mouseY << " last x: " << m_mouseLastX << " last y: " << m_mouseLastY << " mouse x offset: " << xoffset << " mouse y offset: " << yoffset << std::endl;
		//return;

		m_yaw += xoffset;
		m_pitch += yoffset;

		std::cout << yoffset << std::endl;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	void processMouseScroll(GLFWwindow* window, const float deltaTime)
	{
		if (m_zoom >= 1.0f && m_zoom <= 45.0f)
			m_zoom -= 0;
		if (m_zoom <= 1.0f)
			m_zoom = 1.0f;
		if (m_zoom >= 45.0f)
			m_zoom = 45.0f;
	}

	void move(glm::vec3 deltaPos)
	{
		m_position += deltaPos;
	}
private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
};

#endif
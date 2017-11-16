#pragma once

class Camera
{
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_fieldOfView;
public:
	Camera(glm::vec3 position, float fov = 45, float yaw = 0, float pitch = 0, float roll = 0)
		: m_position(position),
		m_worldUp(glm::vec3(0, 1, 0)),
		m_yaw(yaw),
		m_pitch(pitch),
		m_roll(roll),
		m_fieldOfView(fov)
	{
		updateViewMatrix();
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);;
	}

	void updateViewMatrix()
	{
		m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		m_front.y = glm::sin(glm::radians(m_yaw));
		m_front.z = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		m_front = glm::normalize(m_front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_front, m_right));
	}

	
};
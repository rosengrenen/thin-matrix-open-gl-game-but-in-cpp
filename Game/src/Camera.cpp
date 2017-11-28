#include "Camera.h"

void Camera::updateVectors()
{
	m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	m_front.y = glm::sin(glm::radians(m_yaw));
	m_front.z = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

Camera::Camera(Player& player, const glm::vec3& position, float yaw, float pitch, float roll, float fov)
	: m_position(position),
	m_worldUp(glm::vec3(0, 1, 0)),
	m_yaw(yaw),
	m_pitch(pitch),
	m_roll(roll),
	m_fieldOfView(fov),
	m_nearPlane(0.1f),
	m_farPlane(4000.0f),
	m_player(player)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_position - m_front, m_up);
	//return glm::lookAt(m_position, m_player.m_position, m_up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(m_fieldOfView, Window::getAspectRatio(), m_nearPlane, m_farPlane);
}

void Camera::rotate(float yaw, float pitch, float roll)
{
	m_yaw += yaw;
	m_pitch += pitch;
	m_roll += roll;

	if (m_yaw > 89.0f)
	{
		m_yaw = 89.0f;
	}
	else if (m_yaw < 1.0f)
	{
		m_yaw = 1.0f;
	}
}

void Camera::zoom(float value)
{
	m_distanceFromPlayer += value;
}

void Camera::calcCamPos()
{
	float theta = m_pitch - m_player.m_rotation.y;
	m_front.x = glm::cos(glm::radians(theta)) * glm::cos(glm::radians(m_yaw));
	m_front.y = glm::sin(glm::radians(m_yaw));
	m_front.z = glm::sin(glm::radians(theta)) * glm::cos(glm::radians(m_yaw));
	m_front = glm::normalize(m_front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
	m_position = m_player.m_position + m_front * m_distanceFromPlayer;
}

glm::vec3& Camera::getPosition()
{
	return m_position;
}

void Camera::invertPitchAndPlayer()
{
	m_yaw *= -1;;
	m_player.m_position.y *= -1;
}
#pragma once

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include "Player.h"
#include "Window.h"

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
	float m_nearPlane;
	float m_farPlane;

	Player& player;
	float distanceFromPlayer = 50;
	float angleAroundPlayer = 0;
public:
	void updateVectors();
public:
	Camera(Player& player, const glm::vec3& position, float yaw = 0, float pitch = 0, float roll = 0, float fov = 45);

	glm::mat4 getViewMatrix() const;

	glm::mat4 getProjectionMatrix() const;

	void rotate(float yaw, float pitch = 0.0f, float roll = 0.0f);

	void zoom(float value);

	void calcCamPos();

	glm::vec3 getPosition() const;
};
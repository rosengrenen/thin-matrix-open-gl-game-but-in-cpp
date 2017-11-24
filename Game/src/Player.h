#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	const float RUN_SPEED = 80.0f / 60.0f; //TODO: Get time instead of divide
	const float TURN_SPEED = 160.0f / 60.0f;
	const float GRAVITY = -50.0f;
	const float JUMP_POWER = 30.0f;
	float TERRAIN_HEIGHT = 0;
	float curSpeed = 0;
	float curTurnSpeed = 0;
	float scale;
	float upSpeed;
	bool inAir = false;
public:
	Player(const TexturedModel& model, const glm::vec3& position, const glm::vec3& rotation, float scale) : 
		Entity(model, position, rotation, scale)
	{
	}

	void moveFront()
	{
		move(glm::sin(glm::radians(m_rotation.y)) * RUN_SPEED, 0, glm::cos(glm::radians(m_rotation.y)) * RUN_SPEED);
	}

	void moveBack()
	{
		move(-glm::sin(glm::radians(m_rotation.y)) * RUN_SPEED, 0, -glm::cos(glm::radians(m_rotation.y)) * RUN_SPEED);
	}

	void rotateACW()
	{
		rotate(0, TURN_SPEED, 0);
	}

	void rotateCW()
	{
		rotate(0, -TURN_SPEED, 0);
	}

	void jump()
	{
		if (!inAir)
		{
			upSpeed = JUMP_POWER;
			inAir = true;
		}
	}

	void moveP()
	{
		upSpeed += GRAVITY / 30.0f;
		move(0, upSpeed / 30.0f, 0);
		if (m_position.y < TERRAIN_HEIGHT)
		{
			m_position.y = TERRAIN_HEIGHT;
			upSpeed = 0;
			inAir = false;
		}
	}
};
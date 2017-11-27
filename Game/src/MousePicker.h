#pragma once

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Camera.h"
#include "Mouse.h"
#include "Terrain.h"

class MousePicker
{
public:

	const int RECURSION_COUNT = 200;
	const float RAY_RANGE = 600;
	glm::vec3 currentRay;
	Camera& camera;

	Terrain& terrain;
	glm::vec3 currentTerrainPoint;
public:
	MousePicker(Camera& camera, Terrain& terrain) : camera(camera), terrain(terrain)
	{ }

	void update()
	{
		currentRay = getCurrentRay();
		if (intersectionInRange(0, RAY_RANGE, currentRay))
		{
			currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, currentRay);
		}
		else
		{
			currentTerrainPoint = glm::vec3(0);
		}
	}

	glm::vec3 getCurrentRay()
	{
		float mouseX = Mouse::getPosX();
		float mouseY = 600.0f - Mouse::getPosY();
		float ndcX = (2.0f * mouseX) / 800.0f - 1.0f;
		float ndcY = (2.0f * mouseY) / 600.0f - 1.0f;

		glm::vec2 normalizedCoords(ndcX, ndcY);
		glm::vec4 rayClip(normalizedCoords.x, normalizedCoords.y, -1.0f, 1.0f);
		glm::vec4 rayEye = glm::inverse(camera.getProjectionMatrix()) * rayClip;
		rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
		glm::vec3 rayWorld = glm::inverse(camera.getViewMatrix()) * rayEye;
		rayWorld = glm::normalize(rayWorld);
		return rayWorld;
	}

	/****************************************************************************************/

	glm::vec3 getPointOnRay(const glm::vec3& ray, float distance)
	{
		glm::vec3 camPos = camera.getPosition();
		glm::vec3 start(camPos.x, camPos.y, camPos.z);
		glm::vec3 scaledRay(ray.x * distance, ray.y * distance, ray.z * distance);
		return start + scaledRay;
	}

	glm::vec3 binarySearch(int count, float start, float finish, const glm::vec3& ray)
	{
		float half = start + ((finish - start) / 2.0f);
		if (count >= RECURSION_COUNT)
		{
			glm::vec3 endPoint = getPointOnRay(ray, half);

			return endPoint;
		}
		if (intersectionInRange(start, half, ray))
		{
			return binarySearch(count + 1, start, half, ray);
		}
		else
		{
			return binarySearch(count + 1, half, finish, ray);
		}
	}

	bool intersectionInRange(float start, float finish, const glm::vec3& ray)
	{
		glm::vec3 startPoint = getPointOnRay(ray, start);
		glm::vec3 endPoint = getPointOnRay(ray, finish);
		if (!isUnderGround(startPoint) && isUnderGround(endPoint))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isUnderGround(const glm::vec3& testPoint)
	{
		float height = 0;
		if (testPoint.x < 0 || testPoint.x >= 800.0f || testPoint.z < 0 || testPoint.z >= 800.0f)
		{
		}
		else
		{

			height = terrain.getHeightOfTerrain(testPoint.x, testPoint.z);
		}

		if (testPoint.y < height)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
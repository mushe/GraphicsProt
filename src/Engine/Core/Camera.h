#pragma once
#include <glm/glm.hpp>

#include "Core/Transform.h"

struct Camera
{
	Transform transform_ = { glm::vec3(0), glm::vec3(0), glm::vec3(0) };
	float fov_ = 49.0f;
	float near_ = 0.01f;
	float far_ = 300.0f;
	float aspectRatio_ = 16.0f / 9.0f;
};

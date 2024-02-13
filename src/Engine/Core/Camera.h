#pragma once
#include "Core/Common.h"
#include "Core/Transform.h"

struct Camera
{
	Transform transform_ = { Vec3(0), Vec3(0), Vec3(0) };
	float fov_ = 49.0f;
	float near_ = 0.01f;
	float far_ = 300.0f;
	float aspectRatio_ = 1.0f;
};

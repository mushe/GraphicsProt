#pragma once
#include <glm/glm.hpp>
#include "../Engine/Core/Common.h"

class Shape
{
public:
	virtual bool Intersect(const Shape& other) = 0;
};
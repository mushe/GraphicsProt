#pragma once
#include "Core/Camera.h"

namespace OrbitalControl
{
    void Update(Camera& camera);
    void SetOrbit(float theta, float phi, float radius);
}

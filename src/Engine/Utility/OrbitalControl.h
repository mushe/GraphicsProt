#pragma once
#include "Core/Camera.h"

namespace OrbitalControl
{
    void Update(Camera& camera, float thetaOffs = 0.0f, float phiOffs = 0.0f);
    void SetOrbit(float theta, float phi, float radius);
    void SetRadius(float radius);
    void LogOrbit();
}

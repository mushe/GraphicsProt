#include "OrbitalControl.h"
#include "IO/Input.h"

namespace OrbitalControl
{
    float wheelVel = 0.0f;
    glm::vec2 rightMouseVel = glm::vec2(0.0f);
    float orbitalTheta = 1.0f;
    float orbitalPhi = 7.16;
    float orbitalRadius = 15.5f;
}

// camera orbital control
void OrbitalControl::Update(Camera& camera)
{
    auto rightMouseDrag = Input::LeftMouseDrag();
    rightMouseVel += rightMouseDrag * 0.005f;
    rightMouseVel *= 0.9f;

    wheelVel += -Input::MouseWheelDelta() * 0.2f;
    wheelVel *= 0.85f;

    orbitalTheta += -rightMouseVel.y * 0.1f;
    orbitalTheta = glm::clamp(orbitalTheta, 0.01f, 3.14f);
    orbitalPhi += rightMouseVel.x * 0.1f;
    orbitalRadius += wheelVel;
    camera.transform_.position = glm::vec3
    (
        orbitalRadius * sin(orbitalTheta) * cos(orbitalPhi),
        orbitalRadius * cos(orbitalTheta),
        orbitalRadius * sin(orbitalTheta) * sin(orbitalPhi)
    );
    camera.transform_.rotation = -camera.transform_.position;
}
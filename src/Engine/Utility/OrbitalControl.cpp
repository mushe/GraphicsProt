#include "OrbitalControl.h"
#include "IO/Input.h"

namespace OrbitalControl
{
    float wheelVel = 0.0f;
    Vec2 rightMouseVel = Vec2(0.0f);
    float orbitalTheta = 0.95f;
    float orbitalPhi =7.0f;
    float orbitalRadius = 17.0f;
}

// camera orbital control
void OrbitalControl::Update(Camera& camera,float thetaOffs, float phiOffs)
{
    auto rightMouseDrag = Input::LeftMouseDrag();
    rightMouseVel += rightMouseDrag * 0.005f;
    rightMouseVel *= 0.9f;

    wheelVel += -Input::MouseWheelDelta() * 0.2f;
    wheelVel *= 0.85f;

    orbitalTheta += -rightMouseVel.y * 0.1f + thetaOffs;
    orbitalTheta = glm::clamp(orbitalTheta, 0.01f, 3.14f);
    orbitalPhi += rightMouseVel.x * 0.1f + phiOffs;
    orbitalRadius += wheelVel;
    camera.transform_.position = Vec3
    (
        orbitalRadius * sin(orbitalTheta) * cos(orbitalPhi),
        orbitalRadius * cos(orbitalTheta),
        orbitalRadius * sin(orbitalTheta) * sin(orbitalPhi)
    );
    camera.transform_.rotation = -camera.transform_.position;
}

void OrbitalControl::SetOrbit(float theta, float phi, float radius)
{
	orbitalTheta = theta;
	orbitalPhi = phi;
	orbitalRadius = radius;
}

void OrbitalControl::SetPhi(float phi)
{
    orbitalPhi = phi;
}

void OrbitalControl::SetRadius(float radius)
{
    orbitalRadius = radius;
}

void OrbitalControl::LogOrbit()
{
   Debug::Log(to_string(orbitalTheta) + " " + to_string(orbitalPhi) + " " + to_string(orbitalRadius));
}

void OrbitalControl::Reset()
{
    wheelVel = 0.0f;
    rightMouseVel = Vec2(0.0f);
    orbitalTheta = 0.95f;
    orbitalPhi = 7.0f;
    orbitalRadius = 17.0f;
}
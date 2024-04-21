#pragma once
#include "Core/Scene.h"

struct Boid2D
{
    Vec2 pos;
    Vec2 vel;
    Vec4 color;
};

class Boids2dCPU : public Scene
{
public:
    Boids2dCPU()
    {
        Debug::Log("Boids2dCPU()");
    }
    virtual ~Boids2dCPU() { Debug::Log("~Boids2dCPU()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:

    const int boidsCount = 500;
    vector<Boid2D> boids;

    // parameters
    float boidScale = 0.4f;
    float maxVelocity = 0.15f;
    float alignmentRadius = 0.1f;
    float cohesionRadius = 0.13f;
    float separationRadius = 0.035f;
    float alignmentWeight = 0.013f;
    float cohesionWeight = 0.09f;
    float separationWeight = 0.95f;
};
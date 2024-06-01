#pragma once
#include "Core/Scene.h"

struct Particle
{
    Vec2 position = Vec2(0.5f, 0.5f);
    Vec2 velocity = Vec2(0.0f, 0.0f);
    float lifeTime = 1.0f;
    float age = 0.0f;
    float scale = 1.0f;
};

class Hover : public Scene
{
public:
    Hover():
        gravity_(-9.8f),
        initialPosition_(0.5f, 0.75f),
        initialVelocity_(200.0f),
        hoverForce_(8.0f),
        rotationalForce_(40.0f),
        rotationalVelocityAttenuation_(0.95f),
        particleSpawnRate_(5.0f),
        position_(initialPosition_),
        rotation_(0.0f),
        velocity_(initialVelocity_ * 0.0001f),
        rotationalVelocity_(0.0f)
    {
    }
    virtual ~Hover() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    Particle SpawnParticle(Vec2 position);

    // parameters
    float gravity_;
    Vec2 initialPosition_;
    float initialVelocity_;
    float hoverForce_;
    float rotationalForce_;
    float rotationalVelocityAttenuation_;
    float particleSpawnRate_;

    // variables
    Vec2 position_;
    float rotation_;
    float velocity_;
    float rotationalVelocity_;

    // jet particle
    vector<Particle> particles_;
};
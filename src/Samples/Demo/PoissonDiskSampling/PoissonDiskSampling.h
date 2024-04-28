#pragma once
#include "Core/Scene.h"


class PoissonDiskSampling : public Scene
{
public:
    PoissonDiskSampling():
        t_(0.0f),
        r_(0.05f),
        k_(30.0f)
    {
        Debug::Log("PoissonDiskSampling()");
    }
    virtual ~PoissonDiskSampling() { Debug::Log("~PoissonDiskSampling()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    vector<Vec2> PoissonDiskSampling2D(float r, int k, Vec2 area);

    float r_;
    float k_;
    float t_;

    vector<Vec2> positions_;
    vector<Vec4> colors_;
};
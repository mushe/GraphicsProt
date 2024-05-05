#pragma once
#include "Core/Scene.h"


class WhiteNoiseGPU : public Scene
{
public:
    WhiteNoiseGPU()
    {
    }
    virtual ~WhiteNoiseGPU() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<RenderTarget> rt_;
    shared_ptr<Material> postProcessMat_;
    Camera camera_;
};
#pragma once
#include "Core/Scene.h"


struct PerlinNoiseUBO :public UniformBufferBase
{
    float scale = 10.0f;
    float speed = 3.2f;
};


class PerlinNoise : public Scene
{
public:
    PerlinNoise()
    {
        Debug::Log("PerlinNoise()");
    }
    virtual ~PerlinNoise() { Debug::Log("~PerlinNoise()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<RenderTarget> rt_;
    shared_ptr<Material> postProcessMat_;
    PerlinNoiseUBO ubo_;
};
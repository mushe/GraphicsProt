#pragma once
#include "Core/Scene.h"

struct KaleidoscopePostProcessUBO :public UniformBufferBase
{
    float division = 16.0f;
};

class Kaleidoscope : public Scene
{
public:
    Kaleidoscope():t_(0.0f)
    {
    }
    virtual ~Kaleidoscope() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    shared_ptr<Texture> tex_;
    shared_ptr<Material> postProcessMat_;
    shared_ptr<RenderTarget> rt_;
    Camera camera_;
    KaleidoscopePostProcessUBO ubo_;
    float t_;
};
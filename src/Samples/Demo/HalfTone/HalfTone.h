#pragma once
#include "Core/Scene.h"

struct HalfTonePostProcessUBO :public UniformBufferBase
{
    float luminanceMultiply = 1.54f;
    float luminancePower = 1.52f;
};

class HalfTone : public Scene
{
public:
    HalfTone()
    {
    }
    virtual ~HalfTone() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> mesh_;
    shared_ptr<Texture> tex_;
    shared_ptr<Material> mat_;
    shared_ptr<RenderTarget> rt_;
    shared_ptr<Material> postProcessMat_;
    HalfTonePostProcessUBO ubo_;
    Camera camera_;
};
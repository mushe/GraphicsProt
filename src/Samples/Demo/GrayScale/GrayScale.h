#pragma once
#include "Core/Scene.h"

struct PostProcessUBO :public UniformBufferBase
{
    float postProcessLerp = 1.0f;
};

class GrayScale : public Scene
{
public:
    GrayScale()
    {
        Debug::Log("GrayScale()");
    }
    virtual ~GrayScale() { Debug::Log("~GrayScale()"); }

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
    PostProcessUBO ubo_;
};
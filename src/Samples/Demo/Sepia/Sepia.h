#pragma once
#include "Core/Scene.h"

struct SepiaPostProcessUBO :public UniformBufferBase
{
    float postProcessLerp = 1.0f;
};

class Sepia : public Scene
{
public:
    Sepia()
    {
        Debug::Log("Sepia()");
    }
    virtual ~Sepia() { Debug::Log("~Sepia()"); }

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
    SepiaPostProcessUBO ubo_;
};
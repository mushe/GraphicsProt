#pragma once
#include "Core/Scene.h"

struct ChromaticAberrationPostProcessUBO :public UniformBufferBase
{
    float distance = 0.005f;
};

class ChromaticAberration : public Scene
{
public:
    ChromaticAberration(){ }
    virtual ~ChromaticAberration(){ }

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
    ChromaticAberrationPostProcessUBO ubo_;
};
#pragma once
#include "Core/Scene.h"

struct VhsUBO :public UniformBufferBase
{
    float scale = 3.3f;
    float leapThreshold = 0.40f;
    float leapScale = 12.0f;
    float leapSpeed = 3.5f;
};

struct ChromaticAberrationUBO :public UniformBufferBase
{
    float distance = 0.011f;
};

class VHS : public Scene
{
public:
    VHS()
    {
        Debug::Log("VHS()");
    }
    virtual ~VHS() { Debug::Log("~VHS()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    shared_ptr<Material> chromaticAberrationMat_;
    shared_ptr<Material> vhsMat_;
    shared_ptr<Texture> tex_;
    shared_ptr<RenderTarget> rt1_;
    shared_ptr<RenderTarget> rt2_;
    Camera camera_;
    VhsUBO vhsUbo_;
    ChromaticAberrationUBO chromaUbo_;
};
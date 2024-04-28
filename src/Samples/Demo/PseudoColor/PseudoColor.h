#pragma once
#include "Core/Scene.h"

struct PseudoColorUBO :public UniformBufferBase
{
    float colorLerp = 1.0f;
};

class PseudoColor : public Scene
{
public:
    PseudoColor()
    {
        Debug::Log("PseudoColor()");
    }
    virtual ~PseudoColor() { Debug::Log("~PseudoColor()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    Camera camera_;
    shared_ptr<Texture> tex_;
    shared_ptr<Mesh> colorMesh_;
    shared_ptr<Material> colorMat_;
    PseudoColorUBO ubo_;
    shared_ptr<Mesh> grayMesh_;
    shared_ptr<Material> grayMat_;
};
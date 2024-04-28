#pragma once
#include "Core/Scene.h"

struct PosterizationUBO :public UniformBufferBase
{
    float resolution = 0.081f;
};

class Posterization : public Scene
{
public:
    Posterization()
    {
        Debug::Log("Posterization()");
    }
    virtual ~Posterization() { Debug::Log("~Posterization()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Texture> tex_;
    shared_ptr<Mesh> colorMesh_;
    shared_ptr<Material> colorMat_;
    PosterizationUBO ubo_;
    shared_ptr<Mesh> grayMesh_;
    shared_ptr<Material> grayMat_;
    Camera camera_;
};
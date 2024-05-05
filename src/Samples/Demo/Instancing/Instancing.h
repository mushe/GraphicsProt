#pragma once
#include "Core/Scene.h"

struct CustomUBO :public UniformBufferBase
{
    float scale = 1.0f;
};

struct InstancingBuffer : InstancingUniformBufferBase
{
    Vec3 pos = Vec3(0);
    float scale = 1.0f;
};

class Instancing : public Scene
{
public:
    Instancing():instanceCount_(10000)
    {
    }
    virtual ~Instancing() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    Camera camera_;
    CustomUBO ubo_;
    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    int instanceCount_;
    vector<InstancingBuffer> instancingBuffer;
};
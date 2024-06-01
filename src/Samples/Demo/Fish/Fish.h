#pragma once
#include "Core/Scene.h"

#include "FishComputeShader.h"

struct FishUBO : InstancingUniformBufferBase
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 vel;
    alignas(16) glm::vec3 rgb;
};

struct FishCustomUBO :public UniformBufferBase
{
    float dummy = 5.0f;
};

class Fish : public Scene
{
public:
    Fish():
        desiredFishCount_(30000),
        fishCount_(desiredFishCount_ - desiredFishCount_ % 256),
        fieldScale_(1.0f)
    {
    }
    virtual ~Fish() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:

    FishComputeShader computeShader_;
    uint32_t desiredFishCount_;
    uint32_t fishCount_;
    float fieldScale_;
    vector<FishUBO> fishUBO_;

    // shareing buffer between compute shader and instancing shader
    VkBuffer sharingBuffer_;
    VkDeviceMemory sharingBufferMemory_;

    Camera camera_;
    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    shared_ptr<Texture> fishTex_;

    FishCustomUBO customUbo_;
};
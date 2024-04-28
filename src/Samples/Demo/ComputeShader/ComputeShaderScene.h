#pragma once
#include "Core/Scene.h"

#include "ComputeShader.h"
#include "ComputeRenderer.h"

struct ComputeUBO : InstancingUniformBufferBase
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 vel;
    alignas(16) glm::vec3 rgb;
};

class ComputeShaderScene : public Scene
{
public:
    ComputeShaderScene():
        desiredFishCount_(30000),
        fishCount_(desiredFishCount_ - desiredFishCount_ % 256),
        fieldScale_(1.0f)
    {
        Debug::Log("ComputeShaderScene()");
    }
    virtual ~ComputeShaderScene() { Debug::Log("~ComputeShaderScene()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:

    ComputeShader computeShader_;
    ComputeRenderer computeRenderer_;
    uint32_t desiredFishCount_;
    uint32_t fishCount_;
    float fieldScale_;
    vector<ComputeUBO> computeUBO_;

    // shareing buffer between compute shader and instancing shader
    VkBuffer sharingBuffer_;
    VkDeviceMemory sharingBufferMemory_;
};
#include "Core/Engine.h"

#include "ComputeShader.h"
#include "ComputeRenderer.h"

struct ComputeUBO : InstancingUniformBufferBase
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 vel;
    alignas(16) glm::vec3 rgb;
};


int main()
{
    auto engine = Engine::Init();

    ComputeShader computeShader;
    ComputeRenderer computeRenderer;

    const uint32_t desiredFishCount = 30000;
    const uint32_t fishCount = desiredFishCount - desiredFishCount % 256;
    const float fieldScale = 1.0f;

    // shareing buffer between compute shader and instancing shader
    VkBuffer sharingBuffer;
    VkDeviceMemory sharingBufferMemory;
        
    // ----------------------------------------------------------------
    // create compute shader buffer
    std::default_random_engine rndEngine((unsigned)time(nullptr));
    std::uniform_real_distribution<float> rndDist(0.0f, fieldScale);
    std::uniform_real_distribution<float> rNorm(-1.0f, 1.0f);

    std::vector<ComputeUBO> computeUBO(fishCount);
    for (auto& u : computeUBO)
    {
        u.pos = glm::vec3(rndDist(rndEngine) * fieldScale, rndDist(rndEngine) * fieldScale,  rndDist(rndEngine) * fieldScale);
        u.vel = glm::vec3(rNorm(rndEngine), rNorm(rndEngine),  rNorm(rndEngine)) * 0.003f;
        u.rgb = glm::vec3(rndDist(rndEngine), rndDist(rndEngine),  rndDist(rndEngine));
    }

    VkDeviceSize bufferSize = sizeof(ComputeUBO) * fishCount;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, computeUBO.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanCore::GetDevice(), stagingBufferMemory);

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingBuffer, sharingBufferMemory);
    VulkanUtil::CopyBuffer(stagingBuffer, sharingBuffer, bufferSize);

    vkDestroyBuffer(VulkanCore::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), stagingBufferMemory, nullptr);
    // ----------------------------------------------------------------


    // init compute shader and renderer
    computeShader.Init(fishCount, sharingBuffer);
    computeRenderer.Init(fishCount, sharingBuffer);


    // main rendering loop
    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        // execute compute shaderx  
        computeShader.Execute();

        engine->BeginRenderToScreen();

        // render fishes
        computeRenderer.Draw();

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    // terminate
    computeShader.Release();
    computeRenderer.Release();

    engine->Terminate();
}

#include "Fish.h"

void Fish::Start(shared_ptr<Engine> engine)
{
    computeUBO_.resize(fishCount_);

    // ----------------------------------------------------------------
    // create compute shader buffer
    std::default_random_engine rndEngine((unsigned)time(nullptr));
    std::uniform_real_distribution<float> rndDist(0.0f, fieldScale_);
    std::uniform_real_distribution<float> rNorm(-1.0f, 1.0f);

    for (auto& u : computeUBO_)
    {
        u.pos = glm::vec3(rndDist(rndEngine) * fieldScale_, rndDist(rndEngine) * fieldScale_, rndDist(rndEngine) * fieldScale_);
        u.vel = glm::vec3(rNorm(rndEngine), rNorm(rndEngine), rNorm(rndEngine)) * 0.003f;
        u.rgb = glm::vec3(rndDist(rndEngine), rndDist(rndEngine), rndDist(rndEngine));
    }

    VkDeviceSize bufferSize = sizeof(ComputeUBO) * fishCount_;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, computeUBO_.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanCore::GetDevice(), stagingBufferMemory);

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingBuffer_, sharingBufferMemory_);
    VulkanUtil::CopyBuffer(stagingBuffer, sharingBuffer_, bufferSize);

    vkDestroyBuffer(VulkanCore::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), stagingBufferMemory, nullptr);
    // ----------------------------------------------------------------


    // init compute shader and renderer
    computeShader_.Init(fishCount_, sharingBuffer_);
    computeRenderer_.Init(fishCount_, sharingBuffer_);
}


bool Fish::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    // execute compute shader
    computeShader_.Execute();

    engine->BeginRenderToScreen();

    // render fishes
    computeRenderer_.Draw();

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Fish::Release()
{
    computeRenderer_.Release();
    computeShader_.Release();

    vkDestroyBuffer(VulkanCore::GetDevice(), sharingBuffer_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), sharingBufferMemory_, nullptr);
}

#pragma once
#include <stdio.h>
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct InstanceParameters
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 vel;
    alignas(16) glm::vec3 rgb;
};

class ComputeShader
{

public:
    void Init(int particleCount, VkBuffer storageBuffer);
    void Execute();
    void Release();
    
private:
    int particleCount_ = 0;

    VkBuffer storageBuffer_;
    VkCommandBuffer commandBuffer_;

    VkSemaphore semaphore_;
    VkFence fence_;
    
    VkDescriptorSetLayout descriptorSetLayout_;
    VkPipelineLayout pipelineLayout_;
    VkPipeline pipeline_;
    VkDescriptorPool descriptorPool_;
    VkDescriptorSet descriptorSet_;
    
    void InitDescriptorSetLayout();
    void InitPipeline();
    void InitDescriptorPool();
    void InitDescriptorSets();
    void InitCommandBuffers();
    void InitFenceAndSemaphore();
};

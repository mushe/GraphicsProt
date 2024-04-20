#include "ComputeShader.h"
#include "Core/VulkanCore.h"
#include "Core/VulkanUtil.h"
#include "IO/File.h"

void ComputeShader::Init(int particleCount, VkBuffer storageBuffer)
{
    particleCount_ = particleCount;
    storageBuffer_ = storageBuffer;

    InitDescriptorSetLayout();
    InitPipeline();
    InitDescriptorPool();
    InitDescriptorSets();
    InitCommandBuffers();
    InitFenceAndSemaphore();
}


void ComputeShader::Execute()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    vkWaitForFences(VulkanCore::GetDevice(), 1, &fence_, VK_TRUE, UINT64_MAX);
    vkResetFences(VulkanCore::GetDevice(), 1, &fence_);
    vkResetCommandBuffer(commandBuffer_, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    assert(vkBeginCommandBuffer(commandBuffer_, &beginInfo) == VK_SUCCESS);

    vkCmdBindPipeline(commandBuffer_, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_);
    vkCmdBindDescriptorSets(commandBuffer_, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout_, 0, 1, &descriptorSet_, 0, nullptr);

    // execute compute shader
    vkCmdDispatch(commandBuffer_, particleCount_ / 256, 1, 1);
    assert(vkEndCommandBuffer(commandBuffer_) == VK_SUCCESS);
    
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer_;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphore_;

    assert(vkQueueSubmit(VulkanCore::GetQueue(), 1, &submitInfo, fence_) == VK_SUCCESS);
}


void ComputeShader::Release()
{
    vkDestroyBuffer(VulkanCore::GetDevice(), storageBuffer_, nullptr);
    vkDestroyBuffer(VulkanCore::GetDevice(), uniformBuffer_, nullptr);

    vkDestroyPipeline(VulkanCore::GetDevice(), pipeline_, nullptr);
    vkDestroyPipelineLayout(VulkanCore::GetDevice(), pipelineLayout_, nullptr);
    vkDestroyDescriptorSetLayout(VulkanCore::GetDevice(), descriptorSetLayout_, nullptr);

    vkDestroySemaphore(VulkanCore::GetDevice(), semaphore_, nullptr);
    vkDestroyFence(VulkanCore::GetDevice(), fence_, nullptr);
}


void ComputeShader::InitDescriptorSetLayout()
{
    std::array<VkDescriptorSetLayoutBinding, 3> layoutBindings{};
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layoutBindings[0].pImmutableSamplers = nullptr;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[1].binding = 1;
    layoutBindings[1].descriptorCount = 1;
    layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[1].pImmutableSamplers = nullptr;
    layoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[2].binding = 2;
    layoutBindings[2].descriptorCount = 1;
    layoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[2].pImmutableSamplers = nullptr;
    layoutBindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 3;
    layoutInfo.pBindings = layoutBindings.data();

    assert(vkCreateDescriptorSetLayout(VulkanCore::GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout_) == VK_SUCCESS);
}


void ComputeShader::InitPipeline()
{
    auto computeShaderCode = File::ReadShaderFile("../Shaders/spv/FishComp.spv");

    VkShaderModule computeShaderModule = VulkanUtil::CreateShaderModule(computeShaderCode);

    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShaderModule;
    computeShaderStageInfo.pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

    assert(vkCreatePipelineLayout(VulkanCore::GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) == VK_SUCCESS);

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = pipelineLayout_;
    pipelineInfo.stage = computeShaderStageInfo;

    assert(vkCreateComputePipelines(VulkanCore::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_) == VK_SUCCESS);

    vkDestroyShaderModule(VulkanCore::GetDevice(), computeShaderModule, nullptr);
}


void ComputeShader::InitDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(1);
    
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(1) * 2;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(1);

    assert(vkCreateDescriptorPool(VulkanCore::GetDevice(), &poolInfo, nullptr, &descriptorPool_) == VK_SUCCESS);
}


void ComputeShader::InitDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout_);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
    allocInfo.pSetLayouts = layouts.data();

    assert(vkAllocateDescriptorSets(VulkanCore::GetDevice(), &allocInfo, &descriptorSet_) == VK_SUCCESS);

    VkDescriptorBufferInfo uniformBufferInfo{};
    uniformBufferInfo.buffer = uniformBuffer_;
    uniformBufferInfo.offset = 0;

    std::array<VkWriteDescriptorSet, 3> descriptorWrites{};
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet_;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &uniformBufferInfo;

    VkDescriptorBufferInfo storageBufferInfoLastFrame{};
    storageBufferInfoLastFrame.buffer = storageBuffer_;
    storageBufferInfoLastFrame.offset = 0;
    storageBufferInfoLastFrame.range = sizeof(InstanceParameters) * particleCount_;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = descriptorSet_;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &storageBufferInfoLastFrame;

    VkDescriptorBufferInfo storageBufferInfoCurrentFrame{};
    storageBufferInfoCurrentFrame.buffer = storageBuffer_;
    storageBufferInfoCurrentFrame.offset = 0;
    storageBufferInfoCurrentFrame.range = sizeof(InstanceParameters) * particleCount_;

    descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[2].dstSet = descriptorSet_;
    descriptorWrites[2].dstBinding = 2;
    descriptorWrites[2].dstArrayElement = 0;
    descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorWrites[2].descriptorCount = 1;
    descriptorWrites[2].pBufferInfo = &storageBufferInfoCurrentFrame;

    vkUpdateDescriptorSets(VulkanCore::GetDevice(), 3, descriptorWrites.data(), 0, nullptr);
}


void ComputeShader::InitCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanCore::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    assert(vkAllocateCommandBuffers(VulkanCore::GetDevice(), &allocInfo, &commandBuffer_) == VK_SUCCESS);
}


void ComputeShader::InitFenceAndSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    assert(vkCreateSemaphore(VulkanCore::GetDevice(), &semaphoreInfo, nullptr, &semaphore_) == VK_SUCCESS);
    assert(vkCreateFence(VulkanCore::GetDevice(), &fenceInfo, nullptr, &fence_) == VK_SUCCESS);
}
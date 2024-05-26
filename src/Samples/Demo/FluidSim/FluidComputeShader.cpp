#include "FluidComputeShader.h"
#include "Core/VulkanCore.h"
#include "Core/VulkanUtil.h"
#include "IO/File.h"

void FluidComputeShader::Init(int particleCount, VkBuffer storageBuffer)
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


void FluidComputeShader::Execute()
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


void FluidComputeShader::Release()
{
    vkDestroyPipeline(VulkanCore::GetDevice(), pipeline_, nullptr);
    vkDestroyPipelineLayout(VulkanCore::GetDevice(), pipelineLayout_, nullptr);
    vkDestroyDescriptorSetLayout(VulkanCore::GetDevice(), descriptorSetLayout_, nullptr);

    vkDestroySemaphore(VulkanCore::GetDevice(), semaphore_, nullptr);
    vkDestroyFence(VulkanCore::GetDevice(), fence_, nullptr);

    vkFreeDescriptorSets(VulkanCore::GetDevice(), descriptorPool_, 1, &descriptorSet_);
    vkDestroyDescriptorPool(VulkanCore::GetDevice(), descriptorPool_, nullptr);
}


void FluidComputeShader::InitDescriptorSetLayout()
{
    std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings{};
    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    layoutBindings[0].pImmutableSamplers = nullptr;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = layoutBindings.data();

    assert(vkCreateDescriptorSetLayout(VulkanCore::GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout_) == VK_SUCCESS);
}


void FluidComputeShader::InitPipeline()
{
    auto computeShaderCode = File::ReadShaderFile("../Shaders/spv/FluidSimComp.spv");

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


void FluidComputeShader::InitDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;

    assert(vkCreateDescriptorPool(VulkanCore::GetDevice(), &poolInfo, nullptr, &descriptorPool_) == VK_SUCCESS);
}


void FluidComputeShader::InitDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout_);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = layouts.data();

    assert(vkAllocateDescriptorSets(VulkanCore::GetDevice(), &allocInfo, &descriptorSet_) == VK_SUCCESS);

    std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

    VkDescriptorBufferInfo storageBufferInfoCurrentFrame{};
    storageBufferInfoCurrentFrame.buffer = storageBuffer_;
    storageBufferInfoCurrentFrame.offset = 0;
    storageBufferInfoCurrentFrame.range = sizeof(FluidParameters) * particleCount_;

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet_;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &storageBufferInfoCurrentFrame;

    vkUpdateDescriptorSets(VulkanCore::GetDevice(), 1, descriptorWrites.data(), 0, nullptr);//weak
}


void FluidComputeShader::InitCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = VulkanCore::GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    assert(vkAllocateCommandBuffers(VulkanCore::GetDevice(), &allocInfo, &commandBuffer_) == VK_SUCCESS);
}


void FluidComputeShader::InitFenceAndSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    assert(vkCreateSemaphore(VulkanCore::GetDevice(), &semaphoreInfo, nullptr, &semaphore_) == VK_SUCCESS);
    assert(vkCreateFence(VulkanCore::GetDevice(), &fenceInfo, nullptr, &fence_) == VK_SUCCESS);
}
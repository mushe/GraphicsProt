#pragma once

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Utility/Debug.h"
#include "Utility/Singleton.h"

class VulkanCore : public Singleton<VulkanCore>
{
public:
    friend class Singleton<VulkanCore>;
    
    static void Init();
    void CallInit();
    
    static VkInstance GetVulkanInstance()
	{
		assert(GetInstance()->vulkanInstance_ != nullptr && "VulkanInstance not initialized");
		return GetInstance()->vulkanInstance_;
	}

    static VkDevice GetDevice()
    {
        assert(GetInstance()->device_ != nullptr && "VkDevice not initialized");
        return GetInstance()->device_;
    }
    static VkPhysicalDevice GetPhysicalDevice()
    {
        assert(GetInstance()->physicalDevice_ != nullptr && "VkPhysicalDevice not initialized");
        return GetInstance()->physicalDevice_;
    }
    
    static VkRenderPass GetRenderPass()
    {
        assert(GetInstance()->renderPass_ != nullptr && "VkRenderPass not initialized");
        return GetInstance()->renderPass_;
    }
    static VkCommandPool GetCommandPool()
    {
        assert(GetInstance()->commandPool_ != nullptr && "VkCommandPool not initialized");
        return GetInstance()->commandPool_;
    }
    static VkQueue GetQueue()
    {
        assert(GetInstance()->queue_ != nullptr && "VkQueue not initialized");
        return GetInstance()->queue_;
    }
    
    static VkCommandBuffer GetCurrentCommandBuffer()
	{
		assert(GetInstance()->commandBuffers_.size() > 0 && "VkCommandBuffers not initialized");
		return GetInstance()->commandBuffers_[GetInstance()->frameIndex_];
	}

    static VkCommandBuffer* GetCurrentCommandBufferPointer()
    {
        assert(GetInstance()->commandBuffers_.size() > 0 && "VkCommandBuffers not initialized");
        return &GetInstance()->commandBuffers_[GetInstance()->frameIndex_];
    }
    
    static uint32_t GetFrameIndex()
    {
        return GetInstance()->frameIndex_;
    }
    
    static void SetQueue(VkQueue queue)
    {
        GetInstance()->queue_ = queue;
    }
    
    static void SetFrameIndex(uint32_t frameIndex)
    {
        GetInstance()->frameIndex_ = frameIndex;
    }
    

private:
    VkInstance vulkanInstance_ = nullptr;
    VkDevice device_ = nullptr;
    VkPhysicalDevice physicalDevice_ = nullptr;
    
    VkRenderPass renderPass_ = nullptr;
    VkCommandPool commandPool_ = nullptr;
    VkQueue queue_ = nullptr;
    std::vector<VkCommandBuffer> commandBuffers_;
    uint32_t frameIndex_ = 0;

private:
    VulkanCore(){}
    ~VulkanCore(){}
    VulkanCore(const VulkanCore& other) {}

    void InitVulkanInstance(bool enableValidation = false);
    void InitPhysicalDevice();
    void InitLogicalDevice();
    void InitRenderPass();
    void InitCommandPool();
    void InitCommandBuffers();

};

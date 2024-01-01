#pragma once
#include <vulkan/vulkan.hpp>

#include <vector>
#include <iostream>
#include <fstream>

namespace VulkanUtil
{
    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    uint32_t GetMemoryType(uint32_t bits, VkMemoryPropertyFlags properties);

    void CopyBuffer(VkBuffer source, VkBuffer dest, VkDeviceSize size);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void BeginSimpleCommand(VkCommandBuffer& commandBuffer);
    void EndSimpleCommand(VkCommandBuffer& commandBuffer);
};

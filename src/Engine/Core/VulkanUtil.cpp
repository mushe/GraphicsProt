#include "VulkanUtil.h"
#include "Core/VulkanCore.h"

void VulkanUtil::CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    assert(vkCreateImage(VulkanCore::GetDevice(), &imageInfo, nullptr, &image) == VK_SUCCESS);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(VulkanCore::GetDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = GetMemoryType(memRequirements.memoryTypeBits, properties);

    assert(vkAllocateMemory(VulkanCore::GetDevice(), &allocInfo, nullptr, &imageMemory) == VK_SUCCESS);

    vkBindImageMemory(VulkanCore::GetDevice(), image, imageMemory, 0);
}


VkImageView VulkanUtil::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    assert(vkCreateImageView(VulkanCore::GetDevice(), &viewInfo, nullptr, &imageView) == VK_SUCCESS);

    return imageView;
}

// https://github.com/KhronosGroup/Vulkan-Samples/blob/d9a6b1069f8008e83a74ae6c08fc7b0235aa2830/framework/core/device.cpp#L346
uint32_t VulkanUtil::GetMemoryType(uint32_t bits, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(VulkanCore::GetPhysicalDevice(), &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((bits & 1) == 1)
        {
            if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }
        bits >>= 1;
    }

    throw std::runtime_error("Could not find a matching memory type");
}

void VulkanUtil::CopyBuffer(VkBuffer source, VkBuffer dest, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer;
    BeginSimpleCommand(commandBuffer);

    VkBufferCopy region{};
    region.size = size;
    vkCmdCopyBuffer(commandBuffer, source, dest, 1, &region);

    EndSimpleCommand(commandBuffer);
}

void VulkanUtil::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    assert(vkCreateBuffer(VulkanCore::GetDevice(), &bufferInfo, nullptr, &buffer) == VK_SUCCESS);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(VulkanCore::GetDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = GetMemoryType(memRequirements.memoryTypeBits, properties);

    assert(vkAllocateMemory(VulkanCore::GetDevice(), &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS);

    vkBindBufferMemory(VulkanCore::GetDevice(), buffer, bufferMemory, 0);
}


void VulkanUtil::BeginSimpleCommand(VkCommandBuffer& commandBuffer)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = VulkanCore::GetCommandPool();
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(VulkanCore::GetDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
}

void VulkanUtil::EndSimpleCommand(VkCommandBuffer& commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(VulkanCore::GetQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(VulkanCore::GetQueue());

    vkFreeCommandBuffers(VulkanCore::GetDevice(), VulkanCore::GetCommandPool(), 1, &commandBuffer);
}

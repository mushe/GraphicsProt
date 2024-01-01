#include "RenderTexture.h"
#include "Core/VulkanUtil.h"

void RenderTexture::CreateRenderTexture(int width, int height, VkFormat format, VkImageUsageFlags usage)
{
    width_ = width;
    height_ = height;

    VulkanUtil::CreateImage(width, height, format, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage_, textureImageMemory_);
}

void RenderTexture::InitAsColorTexture(int width, int height)
{
    CreateRenderTexture(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    CreateTextureImageView(VK_FORMAT_R8G8B8A8_SRGB);
    CreateTextureSampler();
}

void RenderTexture::InitAsDepthTexture(int width, int height)
{
    CreateRenderTexture(width, height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    CreateTextureImageView(VK_FORMAT_D32_SFLOAT);
    CreateTextureSampler();
}

#include "Texture.h"
#include "IO/File.h"
#include "Core/VulkanUtil.h"

shared_ptr<Texture> Texture::FromPNG(const std::string pngFilePath)
{
	auto texture = Texture::Create();
    texture->CreateTextureImageFromFile(pngFilePath);
    texture->CreateTextureImageView(VK_FORMAT_R8G8B8A8_SRGB);
    texture->CreateTextureSampler();
	return texture;
}

void Texture::CreateTextureImageFromFile(const std::string filePath)
{
    path_ = filePath;
    int w, h, channel;

    //stbi_uc* pixels = stbi_load("../Textures/ocean2.png", &w, &h, &channel, STBI_rgb_alpha);
    auto imageFile = File::ReadImageFile(filePath);
    stbi_uc* pixels = imageFile->data;
    w = imageFile->width;
    h = imageFile->height;
    channel = imageFile->channel;

    width_ = w;
    height_ = h;

    VkDeviceSize imageSize = w * h * 4;
    assert(pixels);


    VkBuffer tempBuffer;
    VkDeviceMemory tempBufferMemory;
    VulkanUtil::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), tempBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(VulkanCore::GetDevice(), tempBufferMemory);

    VulkanUtil::CreateImage(w, h, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage_, textureImageMemory_);

    BlitImageLayout(textureImage_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(tempBuffer, textureImage_, static_cast<uint32_t>(w), static_cast<uint32_t>(h));
    BlitImageLayout(textureImage_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    //stbi_image_free(pixels);
    vkDestroyBuffer(VulkanCore::GetDevice(), tempBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), tempBufferMemory, nullptr);
}

void Texture::CreateTextureImageView(VkFormat format)
{
    textureImageView_ = VulkanUtil::CreateImageView(textureImage_, format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::CreateTextureSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(VulkanCore::GetPhysicalDevice(), &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    assert(vkCreateSampler(VulkanCore::GetDevice(), &samplerInfo, nullptr, &textureSampler_) == VK_SUCCESS);
}


void Texture::BlitImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer;
    VulkanUtil::BeginSimpleCommand(commandBuffer);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags source;
    VkPipelineStageFlags dest;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dest = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        source = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dest = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::runtime_error("could not find image layout!");
    }

    vkCmdPipelineBarrier(commandBuffer, source, dest, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    VulkanUtil::EndSimpleCommand(commandBuffer);
}

void Texture::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer;
    VulkanUtil::BeginSimpleCommand(commandBuffer);
    
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    VulkanUtil::EndSimpleCommand(commandBuffer);
}

void Texture::Release()
{
    vkDestroySampler(VulkanCore::GetDevice(), textureSampler_, nullptr);
    vkDestroyImageView(VulkanCore::GetDevice(), textureImageView_, nullptr);
    vkDestroyImage(VulkanCore::GetDevice(), textureImage_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), textureImageMemory_, nullptr);
}

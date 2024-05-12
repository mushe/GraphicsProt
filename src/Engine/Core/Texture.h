#pragma once
#include <string>
#include <vulkan/vulkan.hpp>
#include "Core/VulkanCore.h"
#include "Utility/Debug.h"

class Texture
{
public:
    Texture() : 
        textureImage_(VK_NULL_HANDLE), 
        textureImageMemory_(VK_NULL_HANDLE), 
        textureImageView_(VK_NULL_HANDLE), 
        textureSampler_(VK_NULL_HANDLE)
    {
        Debug::Log("Texture::Texture()");
        width_ = 0;
        height_ = 0;
    }
    ~Texture()
    {
        Debug::Log("Texture::~Texture()");
        Release();
    }

    static shared_ptr<Texture> Create()
    {
        return std::make_shared<Texture>();
    }

    static shared_ptr<Texture> FromPNG(const std::string pngFilePath);

    VkExtent2D GetExtent() { return { (uint32_t)width_, (uint32_t)height_ }; }

    VkImage* GetImage() { return &textureImage_; }
    VkImageView* GetImageView(){return &textureImageView_;}
    VkSampler* GetSampler() { return &textureSampler_; }

    void CreateTextureImageFromFile(const std::string filePath);
    void CreateTextureImageView(VkFormat format);
    void CreateTextureSampler();

protected:
    void BlitImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void Release();

protected:
    VkImage textureImage_;
    VkDeviceMemory textureImageMemory_;
    VkImageView textureImageView_;
    VkSampler textureSampler_;
    uint32_t width_, height_;

public:
    std::string path_;

};

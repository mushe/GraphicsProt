#pragma once
#include "Core/Texture.h"

class RenderTexture : public Texture
{
public:
    RenderTexture(): Texture() {
    }
    ~RenderTexture() {
    }

    static shared_ptr<RenderTexture> Create()
    {
        return std::make_shared<RenderTexture>();
    }

    void CreateRenderTexture(int width, int height, VkFormat format, VkImageUsageFlags usage);
    void InitAsColorTexture(int width, int height);
    void InitAsDepthTexture(int width, int height);
};
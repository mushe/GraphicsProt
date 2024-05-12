#pragma once
#include <string>
#include <vulkan/vulkan.hpp>

#include "Core/VulkanCore.h"
#include "Core/Texture.h"
#include "Core/RenderTexture.h"
#include "Utility/Debug.h"

class RenderTarget
{
public:
    RenderTarget():
        renderTexture_(nullptr), 
        renderDepthTexture_(nullptr), 
        renderTextureFrameBuffer_(VK_NULL_HANDLE), 
        renderPass_(VK_NULL_HANDLE) 
    {
    }
    ~RenderTarget()
    {
        Release();
    }

    static shared_ptr<RenderTarget> Create(int width, int height)
    {
		auto renderTarget = std::make_shared<RenderTarget>();
		renderTarget->Init(width, height);
		return renderTarget;
	}

    shared_ptr<Texture> GetColorTexture() { return renderTexture_; }
    shared_ptr<Texture> GetDepthTexture() { return renderDepthTexture_; }

    void BeginRenderToTexture();
    void EndRenderToTexture();

private:
    void Init(const int width, const int height);
    void InitRenderPass();
    void Release();

private:
    std::shared_ptr<RenderTexture> renderTexture_;
    std::shared_ptr<RenderTexture> renderDepthTexture_;
    VkFramebuffer renderTextureFrameBuffer_;
    VkRenderPass renderPass_;
};

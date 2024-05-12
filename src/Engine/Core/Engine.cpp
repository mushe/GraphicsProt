#include "Engine.h"
#include <cassert>
#include <thread>

shared_ptr<Engine> Engine::Init(const int windowWidth, const int windowHeight, const int windowPosX, const int windowPosY)
{
    auto engine = Engine::Create();
    engine->windowWidth_ = windowWidth;
    engine->windowHeight_ = windowHeight;
    engine->windowPosX_ = windowPosX;
    engine->windowPosY_ = windowPosY;
    engine->Run();
    return engine;
}

void Engine::Run()
{
    Debug::Log("Engine::Run()");

    InitWindow();

    VulkanCore::Init();

    InitWindowSurface();
    InitQueues();
    PrepareSwapChain();
    InitFenceAndSemaphores();

    imGuiWrapper_.Init(window_, VulkanCore::GetVulkanInstance(), VulkanCore::GetDevice(), VulkanCore::GetPhysicalDevice(), VulkanCore::GetRenderPass(), VulkanCore::GetQueue(), VulkanCore::GetCommandPool());
    GUI::GetInstance()->Init(&imGuiWrapper_);
    Input::GetInstance()->Init(window_);

    InitRenderTexture();
    InitQuadMesh();

    ShapeDrawer::Init();
}

void Engine::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window_ = glfwCreateWindow(windowWidth_, windowHeight_, "GraphicsProt", nullptr, nullptr);
    glfwSetWindowPos(window_, windowPosX_, windowPosY_);
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, FrameBufferSizeCallback);
    glfwSetScrollCallback(window_, Input::MouseScrollCallback);
    glfwSetCursorPosCallback(window_, Input::MouseCursorPositionCallback);
    glfwSetMouseButtonCallback(window_, Input::MouseButtonCallback);
}

void Engine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
    engine->windowResized_ = true;
}

void Engine::OnWindowResized()
{
    PrepareSwapChain();
    windowResized_ = false;
}

void Engine::PrepareSwapChain()
{
    glfwGetFramebufferSize(window_, &windowWidth_, &windowHeight_);
    while (windowWidth_ == 0 || windowHeight_ == 0) // for window minimization
    {
        glfwGetFramebufferSize(window_, &windowWidth_, &windowHeight_);
        glfwWaitEvents();
    }

    // release swapchain
    for (auto swapchainFramebuffer : swapChainFramebuffers_)
        vkDestroyFramebuffer(VulkanCore::GetDevice(), swapchainFramebuffer, nullptr);

    for (auto swapchainImageView : swapChainImageViews_)
        vkDestroyImageView(VulkanCore::GetDevice(), swapchainImageView, nullptr);

    vkDestroySwapchainKHR(VulkanCore::GetDevice(), swapChain_, nullptr);

    // init swapchain
    InitSwapChain();
    InitImageViews();
    InitDepthImage();
    InitFramebuffers();
}

void Engine::BeginFrame()
{
    startTime_ = std::chrono::system_clock::now();

    VulkanCore::SetFrameIndex(frameIndex_);

    // frame polling and input
    glfwPollEvents();

    BeginCommandBuffer();
}

void Engine::EndFrame(const int waitFPS)
{
    EndCommandBuffer();

    SubmitQueue();
    Present();

    frameIndex_ = (frameIndex_ + 1) % maxFrames_;

    Input::GetInstance()->Update();

    // wait for frame time
    FixFPS(waitFPS);
}

void Engine::FixFPS(const int waitFPS)
{
    const std::chrono::milliseconds FRAME_DURATION(1000 / waitFPS);

    endTime_ = std::chrono::system_clock::now();
    
    auto frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_);

    while (frameTime < FRAME_DURATION) 
    {
        endTime_ = std::chrono::system_clock::now();
        frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_ - startTime_);
    }
}

void Engine::BeginCommandBuffer()
{
    vkWaitForFences(VulkanCore::GetDevice(), 1, &objectRenderFences_[frameIndex_], VK_TRUE, UINT64_MAX);
    auto res = vkAcquireNextImageKHR(VulkanCore::GetDevice(), swapChain_, UINT64_MAX, objectRenderSemaphores_[frameIndex_], VK_NULL_HANDLE, &imageIndex_);

    vkResetFences(VulkanCore::GetDevice(), 1, &objectRenderFences_[frameIndex_]);
    vkResetCommandBuffer(VulkanCore::GetCurrentCommandBuffer(), 0);

    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    assert(vkBeginCommandBuffer(VulkanCore::GetCurrentCommandBuffer(), &commandBufferBeginInfo) == VK_SUCCESS);
}

void Engine::BeginRenderToScreen()
{
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = VulkanCore::GetRenderPass();
    renderPassBeginInfo.framebuffer = swapChainFramebuffers_[imageIndex_];
    renderPassBeginInfo.renderArea.offset = { 0, 0 };
    renderPassBeginInfo.renderArea.extent = swapChainExtent_;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(VulkanCore::GetCurrentCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent_.width;
    viewport.height = (float)swapChainExtent_.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(VulkanCore::GetCurrentCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent_;
    vkCmdSetScissor(VulkanCore::GetCurrentCommandBuffer(), 0, 1, &scissor);

    ShapeDrawer::OnBeginRenderToScreen();
}

void Engine::EndRenderToScreen()
{
    ShapeDrawer::OnEndRenderToScreen();

    vkCmdEndRenderPass(VulkanCore::GetCurrentCommandBuffer());
}

void Engine::BeginRenderToTexture()
{
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = VulkanCore::GetRenderPass();
    renderPassBeginInfo.framebuffer = renderTextureFrameBuffer_;
    renderPassBeginInfo.renderArea.offset = { 0, 0 };
    renderPassBeginInfo.renderArea.extent = renderTexture_->GetExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(VulkanCore::GetCurrentCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)renderTexture_->GetExtent().width;
    viewport.height = (float)renderTexture_->GetExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(VulkanCore::GetCurrentCommandBuffer(), 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = renderTexture_->GetExtent();
    vkCmdSetScissor(VulkanCore::GetCurrentCommandBuffer(), 0, 1, &scissor);
}

void Engine::EndRenderToTexture()
{
    vkCmdEndRenderPass(VulkanCore::GetCurrentCommandBuffer());
}

void Engine::EndCommandBuffer()
{
    assert(vkEndCommandBuffer(VulkanCore::GetCurrentCommandBuffer()) == VK_SUCCESS);
}

void Engine::SubmitQueue()
{
    VkSemaphore waitSemaphores[] = { objectRenderSemaphores_[frameIndex_] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = VulkanCore::GetCurrentCommandBufferPointer();
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderingSemaphores_[frameIndex_];
    assert(vkQueueSubmit(renderingQueue_, 1, &submitInfo, objectRenderFences_[frameIndex_]) == VK_SUCCESS);
}

void Engine::Present()
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderingSemaphores_[frameIndex_];

    VkSwapchainKHR swapChains[] = { swapChain_ };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex_;

    auto res = vkQueuePresentKHR(presentQueue_, &presentInfo);
    if (windowResized_)
    {
        OnWindowResized();
        return;
    }
    assert(res == VK_SUCCESS);
}

void Engine::Quit()
{
    closeWindow_ = true;
}

void Engine::Terminate()
{
    Debug::Log("Engine::Terminate()");

    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Engine::Running()
{
	return !glfwWindowShouldClose(window_) && !closeWindow_;
}

void Engine::OnGUI(std::function<void()> callback)
{
    imGuiWrapper_.BeginFrame("Resolution : " + std::to_string(windowWidth_) + " x " + std::to_string(windowHeight_));
    {
        GUI::GetInstance()->parameters_.clear();

        GUI::ShowFPS();
        GUI::Space();
        GUI::Space();
        
        callback();

        //if (ImGui::Button("Print GUI Parameters"))
        //    GUI::PrintParameters();
    }
    imGuiWrapper_.EndFrame(VulkanCore::GetCurrentCommandBuffer());
}

void Engine::InitWindowSurface()
{
    assert(glfwCreateWindowSurface(VulkanCore::GetVulkanInstance(), window_, nullptr, &surface_) == VK_SUCCESS);
}

void Engine::InitQueues()
{
    vkGetDeviceQueue(VulkanCore::GetDevice(), 0, 0, &renderingQueue_);
    vkGetDeviceQueue(VulkanCore::GetDevice(), 0, 0, &computeQueue_);
    vkGetDeviceQueue(VulkanCore::GetDevice(), 0, 0, &presentQueue_);

    VulkanCore::SetQueue(renderingQueue_);
}

void Engine::InitSwapChain()
{
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanCore::GetPhysicalDevice(), surface_, &capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanCore::GetPhysicalDevice(), surface_, &formatCount, nullptr);
    assert(formatCount != 0);

    std::vector<VkSurfaceFormatKHR> formats;
    formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanCore::GetPhysicalDevice(), surface_, &formatCount, formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanCore::GetPhysicalDevice(), surface_, &presentModeCount, nullptr);
    assert(presentModeCount != 0);

    std::vector<VkPresentModeKHR> presentModes;
    presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanCore::GetPhysicalDevice(), surface_, &presentModeCount, presentModes.data());

    VkSurfaceFormatKHR surfaceFormat;
    surfaceFormat.format = VK_FORMAT_R8G8B8A8_SRGB;
    surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    uint32_t imageCount = capabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface_;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = capabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
    createInfo.clipped = VK_TRUE;

    assert(vkCreateSwapchainKHR(VulkanCore::GetDevice(), &createInfo, nullptr, &swapChain_) == VK_SUCCESS);

    vkGetSwapchainImagesKHR(VulkanCore::GetDevice(), swapChain_, &imageCount, nullptr);
    swapChainImages_.resize(imageCount);
    vkGetSwapchainImagesKHR(VulkanCore::GetDevice(), swapChain_, &imageCount, swapChainImages_.data());

    swapChainImageFormat_ = surfaceFormat.format;
    swapChainExtent_ = capabilities.currentExtent;
}

void Engine::InitImageViews()
{
    swapChainImageViews_.resize(swapChainImages_.size());

    for (uint32_t i = 0; i < swapChainImages_.size(); i++)
    {
        swapChainImageViews_[i] = VulkanUtil::CreateImageView(swapChainImages_[i], swapChainImageFormat_, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

void Engine::InitFramebuffers()
{
    swapChainFramebuffers_.resize(swapChainImageViews_.size());

    for (size_t i = 0; i < swapChainImageViews_.size(); i++)
    {
        std::array<VkImageView, 2> attachments =
        {
            swapChainImageViews_[i],
            depthImageView_
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = VulkanCore::GetRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent_.width;
        framebufferInfo.height = swapChainExtent_.height;
        framebufferInfo.layers = 1;

        assert(vkCreateFramebuffer(VulkanCore::GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) == VK_SUCCESS);
    }
}

void Engine::InitRenderTexture()
{
    const int w = 2048;
    const int h = 2048;

    // create texture
    renderTexture_ = RenderTexture::Create();
    renderTexture_->InitAsColorTexture(w, h);

    renderDepthTexture_ = RenderTexture::Create();
    renderDepthTexture_->InitAsDepthTexture(w, h);

    // create frame buffer
    std::array<VkImageView, 2> attachments =
    {
        *renderTexture_->GetImageView(),
        //depthImageView_,
        *renderDepthTexture_->GetImageView(),
    };

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = VulkanCore::GetRenderPass();
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = w;
    framebufferInfo.height = h;
    framebufferInfo.layers = 1;

    assert(vkCreateFramebuffer(VulkanCore::GetDevice(), &framebufferInfo, nullptr, &renderTextureFrameBuffer_) == VK_SUCCESS);
}

void Engine::InitDepthImage()
{
    VulkanUtil::CreateImage(swapChainExtent_.width, swapChainExtent_.height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage_, depthImageMemory_);
    depthImageView_ = VulkanUtil::CreateImageView(depthImage_, VK_FORMAT_D32_SFLOAT, VK_IMAGE_ASPECT_DEPTH_BIT);
}

void Engine::InitFenceAndSemaphores()
{
    objectRenderSemaphores_.resize(maxFrames_);
    renderingSemaphores_.resize(maxFrames_);
    objectRenderFences_.resize(maxFrames_);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < maxFrames_; i++)
    {
        assert(vkCreateSemaphore(VulkanCore::GetDevice(), &semaphoreInfo, nullptr, &objectRenderSemaphores_[i]) == VK_SUCCESS);
        assert(vkCreateSemaphore(VulkanCore::GetDevice(), &semaphoreInfo, nullptr, &renderingSemaphores_[i]) == VK_SUCCESS);
        assert(vkCreateFence(VulkanCore::GetDevice(), &fenceInfo, nullptr, &objectRenderFences_[i]) == VK_SUCCESS);
    }
}

void Engine::InitQuadMesh()
{
    quadMesh_ = Mesh::Create();
    auto quadModel = ModelGenerator::GenerateDisplayQuad();
    quadMesh_->SetVertices(quadModel.vertices);
    quadMesh_->SetIndices(quadModel.indices);
    quadMesh_->Init();
}

void Engine::BlitToRenderTarget(shared_ptr<Material> material, shared_ptr<RenderTarget> dest)
{
    dest->BeginRenderToTexture();
    quadMesh_->SetMaterial(material);
    quadMesh_->Draw(dummyCamera_);
    dest->EndRenderToTexture();
}

void Engine::BlitToScreen(shared_ptr<Material> material)
{
    quadMesh_->SetMaterial(material);
    quadMesh_->Draw(dummyCamera_);
}

void Engine::SetWindowPosition(const int windowPosx, const int windowPosY)
{
	glfwSetWindowPos(window_, windowPosx, windowPosY);
}

void Engine::Release()
{
    quadMesh_ = nullptr;
    renderTexture_ = nullptr;
    renderDepthTexture_ = nullptr;
    File::ReleaseImageCache();

    for (auto swapchainFramebuffer : swapChainFramebuffers_)
        vkDestroyFramebuffer(VulkanCore::GetDevice(), swapchainFramebuffer, nullptr);
    
    for (auto swapchainImageView : swapChainImageViews_)
        vkDestroyImageView(VulkanCore::GetDevice(), swapchainImageView, nullptr);
    
    vkDestroySwapchainKHR(VulkanCore::GetDevice(), swapChain_, nullptr);
    vkDestroyImageView(VulkanCore::GetDevice(), depthImageView_, nullptr);
    vkDestroyFramebuffer(VulkanCore::GetDevice(), renderTextureFrameBuffer_, nullptr);
    vkDestroyImage(VulkanCore::GetDevice(), depthImage_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), depthImageMemory_, nullptr);

    for (int i = 0; i < maxFrames_; i++)
    {
        vkDestroySemaphore(VulkanCore::GetDevice(), objectRenderSemaphores_[i], nullptr);
        vkDestroySemaphore(VulkanCore::GetDevice(), renderingSemaphores_[i], nullptr);
        vkDestroyFence(VulkanCore::GetDevice(), objectRenderFences_[i], nullptr);
    }
}
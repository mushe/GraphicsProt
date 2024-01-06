#pragma once

#include <stdio.h>
#include <cassert>
#include <functional>
#include <chrono>
#include <thread>

#include "Core/Common.h"
#include "Core/Camera.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Core/Texture.h"
#include "Core/RenderTexture.h"
#include "Core/VulkanCore.h"
#include "Core/RenderTarget.h"
#include "Core/VulkanUtil.h"
#include "GUI/ImGuiWrapper.h"
#include "GUI/GUI.h"
#include "IO/Input.h"
#include "IO/ObjParser.h"
#include "IO/File.h"
#include "Utility/Debug.h"
#include "Utility/Random.h"
#include "Utility/Singleton.h"
#include "Utility/ModelGenerator.h"
#include "Utility/OrbitalControl.h"
#include "Utility/ShapeDrawer.h"
#include "Utility/Algorithm.h"

class Engine
{
public:
    Engine() :
        window_(nullptr),
        surface_(VK_NULL_HANDLE),
        renderingQueue_(VK_NULL_HANDLE),
        computeQueue_(VK_NULL_HANDLE),
        presentQueue_(VK_NULL_HANDLE),
        swapChain_(VK_NULL_HANDLE),
        swapChainImageFormat_(VK_FORMAT_UNDEFINED),
        swapChainExtent_{ 0, 0 },
        depthImage_(VK_NULL_HANDLE),
        depthImageMemory_(VK_NULL_HANDLE),
        depthImageView_(VK_NULL_HANDLE),
        renderTextureFrameBuffer_(VK_NULL_HANDLE),
        imGuiWrapper(ImGuiWrapper()),
        elapsedTimeSeconds_(0.0f)
    {
    }

    ~Engine()
    {
        File::ReleaseImageCache();
    }

    static shared_ptr<Engine> Create() { return std::make_shared<Engine>(); }
    static shared_ptr<Engine> Init(const int windowWidth = 1920, const int windowHeight = 1080, const int windowPosX = 0, const int windowPosY = 50);
    void Run();

    float ElapsedTimeSeconds(){return elapsedTimeSeconds_;}
    float FPS(){return 0.0;}
    void WaitForFrame(float fps){}
    void Blit(RenderTexture *dest, RenderTexture *src, Material *material){}
    
    void BeginFrame();
    void EndFrame(const int waitFPS = 60.0f);

    void BeginRenderToScreen();
    void EndRenderToScreen();

    void Terminate();
    bool Running();
    void Quit();

    void OnGUI(std::function<void()> callback);

    void BeginRenderToTexture();
    void EndRenderToTexture();

    void OnWindowResized();
    void PrepareSwapChain();

    void BlitToRenderTarget(shared_ptr<Material> material, shared_ptr<RenderTarget> dest);
    void BlitToScreen(shared_ptr<Material> material);
    void SetWindowPosition(const int windowPosx = 0, const int windowPosY = 50);

// core
private:
    void InitWindow();
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void BeginCommandBuffer();
    void EndCommandBuffer();
    void SubmitQueue();
    void Present();
    void FixFPS(const int waitFPS = 60);

// vulkan initialize
private:
    void InitWindowSurface();
    void InitQueues();
    void InitSwapChain();
    void InitImageViews();
    void InitFramebuffers();
    void InitDepthImage();
    void InitFenceAndSemaphores();
    void InitQuadMesh();
    void InitRenderTexture();

public:
    ImGuiWrapper imGuiWrapper;

    int windowWidth_ = 1920;
    int windowHeight_ = 1080;
    int windowPosX_ = 0;
    int windowPosY_ = 50;
    const int maxFrames_ = 2;

    GLFWwindow* window_;
    VkSurfaceKHR surface_;

    VkQueue renderingQueue_;
    VkQueue computeQueue_;
    VkQueue presentQueue_;

    VkSwapchainKHR swapChain_;
    std::vector<VkImage> swapChainImages_;
    VkFormat swapChainImageFormat_;
    VkExtent2D swapChainExtent_;

    std::vector<VkImageView> swapChainImageViews_;
    std::vector<VkFramebuffer> swapChainFramebuffers_;

    VkImage depthImage_;
    VkDeviceMemory depthImageMemory_;
    VkImageView depthImageView_;

    std::vector<VkSemaphore> objectRenderSemaphores_;
    std::vector<VkSemaphore> renderingSemaphores_;
    std::vector<VkFence> objectRenderFences_;

    uint32_t frameIndex_ = 0;
    uint32_t imageIndex_ = 0;

    bool closeWindow_ = false;

    std::shared_ptr<RenderTexture> renderTexture_;
    std::shared_ptr<RenderTexture> renderDepthTexture_;
    VkFramebuffer renderTextureFrameBuffer_;

    std::chrono::system_clock::time_point startTime_;
    std::chrono::system_clock::time_point endTime_;

    bool windowResized_ = false;

    // quad mesh for screen rendering
    std::shared_ptr<Mesh> quadMesh_;
    Camera dummyCamera_;

    float elapsedTimeSeconds_ = 0.0f;
};

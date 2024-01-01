#pragma once

#include <string>

#include "ThirdParty/imgui/imgui_impl_vulkan.h"
#include "ThirdParty/imgui/imgui_impl_glfw.h"
#include "ThirdParty/imgui/imgui_impl_vulkan.h"

class ImGuiWrapper
{
private:
    VkDescriptorPool _descriptorPool;
public:
    void Init(GLFWwindow* window, VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, VkRenderPass renderPass, VkQueue queue, VkCommandPool commandPool);
    void BeginFrame(std::string guiName);
    void EndFrame(VkCommandBuffer commandBufferToDraw);
    void ShowFPS();
    void ChangeStyleToLight();
};

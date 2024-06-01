#pragma once
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>


class ComputeRenderer
{
private:
    
    struct UniformBufferObject
    {
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };
    
    struct InstanceParameters
    {
        alignas(16) glm::vec3 pos;
        alignas(16) glm::vec3 vel;
        alignas(16) glm::vec3 rgb;
    };
    
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    const std::vector<Vertex> vertices =
    {
        {{-0.12f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.12f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.12f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.12f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices =
    {
        0, 1, 2, 2, 3, 0,
    };
    
    int particleCount_ = 0;
    
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    
    void CreateTextureImage();
    void CreateTextureImageView();
    void CreateTextureSampler();
    
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    
    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    
    VkBuffer storageBuffer_;
    
    VkDescriptorSetLayout descriptorSetLayout_;
    VkPipelineLayout pipelineLayout_;
    VkPipeline pipeline_;
    
    VkBuffer vertexBuffer_;
    VkDeviceMemory vertexBufferMemory_;
    VkBuffer indexBuffer_;
    VkDeviceMemory indexBufferMemory_;
    
    VkImage textureImage_;
    VkDeviceMemory textureImageMemory_;
    VkImageView textureImageView_;
    VkSampler textureSampler_;
    
    VkBuffer uniformBuffer_;
    VkDeviceMemory uniformBufferMemory_;
    void* uniformBufferMapped_;
    VkBuffer instanceUniformBuffer_;
    VkDeviceMemory instanceUniformBufferMemory_;
    void* instanceUniformBufferMapped_;

    VkDescriptorPool descriptorPool_;
    VkDescriptorSet descriptorSets_;
    
public:
    void Init(const int particleCount, VkBuffer storageBuffer);
    void Draw();
    void Release();
    
    float cameraFov_ = 45.0f;
    glm::vec4 cameraPos_ = glm::vec4(1.2f,  0.5f, 0.5f, 0.0f);
    glm::vec4 cameraCenter_ = glm::vec4(0.5f, 0.5f, 0.5f, 0.0f);
};

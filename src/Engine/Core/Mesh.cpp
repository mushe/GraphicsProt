#include "Mesh.h"
#include "VulkanUtil.h"
#include "IO/File.h"
#include "Utility/StopWatch.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>


std::shared_ptr<Mesh> Mesh::Create()
{
    auto mesh = std::make_shared<Mesh>();
    return mesh;
}

std::shared_ptr<Mesh> Mesh::FromOBJ(std::string objFilePath, int face)
{
    auto mesh = std::make_shared<Mesh>();

    ObjParser parser;
    auto objData = parser.Parse(objFilePath, face);
    mesh->SetVertices(objData.vertices);
    mesh->SetIndices(objData.indices);
    mesh->Init();

    return mesh;
}

void Mesh::Init()
{
    CreateVertexBuffer();
    CreateIndexBuffer();
}


void Mesh::CreateVertexBuffer()
{
    VkDeviceSize bufferSize = sizeof(vertices_[0]) * vertices_.size();

    VkBuffer tempBuffer;
    VkDeviceMemory tempBufferMemory;
    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), tempBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices_.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanCore::GetDevice(), tempBufferMemory);

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer_, vertexBufferMemory_);

    VulkanUtil::CopyBuffer(tempBuffer, vertexBuffer_, bufferSize);

    vkDestroyBuffer(VulkanCore::GetDevice(), tempBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), tempBufferMemory, nullptr);
}

void Mesh::CreateIndexBuffer()
{
    VkDeviceSize bufferSize = sizeof(indices_[0]) * indices_.size();

    VkBuffer tempBuffer;
    VkDeviceMemory tempBufferMemory;
    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), tempBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices_.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanCore::GetDevice(), tempBufferMemory);

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer_, indexBufferMemory_);

    VulkanUtil::CopyBuffer(tempBuffer, indexBuffer_, bufferSize);

    vkDestroyBuffer(VulkanCore::GetDevice(), tempBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), tempBufferMemory, nullptr);
}

void Mesh::Draw(const Camera& camera)
{
    VkCommandBuffer commandBuffer = VulkanCore::GetCurrentCommandBuffer();

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material_->pipeline_);


    VkDeviceSize bufferSize = material_->GetUboBufferSize();
    if (bufferSize > 0)
    {
        vkMapMemory(VulkanCore::GetDevice(), material_->uniformBufferMemory_, 0, bufferSize, 0, &material_->uniformBufferMapped_);
        memcpy(material_->uniformBufferMapped_, material_->GetUniformBuffer(), bufferSize);
        vkUnmapMemory(VulkanCore::GetDevice(), material_->uniformBufferMemory_);
    }


    glm::mat4x4 view = glm::lookAt(camera.transform_.position, camera.transform_.position + camera.transform_.rotation, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4x4 proj = glm::perspective(glm::radians(camera.fov_), camera.aspectRatio_, camera.near_, camera.far_);
    proj[1][1] *= -1;

    auto trans = glm::translate(glm::mat4(1), transform_.position);
    auto rot = glm::mat4_cast(glm::quat(transform_.rotation));
    auto scale = glm::scale(glm::mat4(1), transform_.scale);
    glm::mat4x4 world = trans * rot * scale;

    time_ += 0.01f;
    material_->commonUBO_.time = time_;
    material_->commonUBO_.WVP = proj * view * world;

    vkMapMemory(VulkanCore::GetDevice(), material_->commonUniformBufferMemory_, 0, sizeof(CommonUniformBuffer), 0, &material_->commonUniformBufferMapped_);
    memcpy(material_->commonUniformBufferMapped_, &material_->commonUBO_, sizeof(CommonUniformBuffer));
    vkUnmapMemory(VulkanCore::GetDevice(), material_->commonUniformBufferMemory_);


    // Draw
    VkBuffer vertexBuffers[] = { vertexBuffer_ };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material_->pipelineLayout_, 0, 1, &material_->descriptorSets_[VulkanCore::GetFrameIndex()], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices_.size()), 1, 0, 0, 0);
}


void Mesh::Release()
{
    vkDestroyBuffer(VulkanCore::GetDevice(), indexBuffer_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), indexBufferMemory_, nullptr);

    vkDestroyBuffer(VulkanCore::GetDevice(), vertexBuffer_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), vertexBufferMemory_, nullptr);
}

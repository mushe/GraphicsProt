#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <glm/glm.hpp>

#include "Core/VulkanCore.h"
#include "Core/Texture.h"

struct UniformBufferBase
{

};

struct CommonUniformBuffer
{
	glm::mat4 WVP;
	float time;
};

struct VertexData
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoord;
};


struct MaterialInfo
{
	std::string vertShaderPath;
	std::string fragShaderPath;
	int customUiformBufferSize;
	std::vector<shared_ptr<Texture>> textures;
};

class Material
{

public:
	Material():
		commonUBO_(),
		uniformBuffer_(VK_NULL_HANDLE),
		uniformBufferMemory_(VK_NULL_HANDLE),
		commonUniformBuffer_(VK_NULL_HANDLE),
		commonUniformBufferMapped_(VK_NULL_HANDLE),
		commonUniformBufferMemory_(VK_NULL_HANDLE),
		descriptorPool_(VK_NULL_HANDLE),
		descriptorSetLayout_(VK_NULL_HANDLE),
		shaderStages_{},
		vertShaderModule_(VK_NULL_HANDLE),
		fragShaderModule_(VK_NULL_HANDLE),
		customUboBufferSize_(0)
	{}
	~Material() 
	{
		vkDestroyShaderModule(VulkanCore::GetDevice(), fragShaderModule_, nullptr);
		vkDestroyShaderModule(VulkanCore::GetDevice(), vertShaderModule_, nullptr);
	}

	static shared_ptr<Material> Create(MaterialInfo materialInfo);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, std::vector<shared_ptr<Texture>> textures);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, std::vector<shared_ptr<Texture>> textures);
	
	int GetUboBufferSize() { return customUboBufferSize_; }
	VkPipelineShaderStageCreateInfo* GetShaderStages() { return shaderStages_; }
	std::vector<std::shared_ptr<Texture>> GetTextures() { return textures_; }
	void SetUniformBufferData(UniformBufferBase* ubo){ ubo_ = ubo; }
	UniformBufferBase* GetUniformBuffer() { return ubo_; }

public:
	VkBuffer uniformBuffer_;
	VkDeviceMemory uniformBufferMemory_;
	void* uniformBufferMapped_;
	VkDescriptorSetLayout descriptorSetLayout_;
	VkDescriptorPool descriptorPool_;
	std::vector<VkDescriptorSet> descriptorSets_;
	VkPipelineLayout pipelineLayout_;
	VkPipeline pipeline_;

	CommonUniformBuffer commonUBO_;
	VkBuffer commonUniformBuffer_;
	VkDeviceMemory commonUniformBufferMemory_;
	void* commonUniformBufferMapped_;

private:
	int customUboBufferSize_;
	VkPipelineShaderStageCreateInfo shaderStages_[2];
	std::vector<std::shared_ptr<Texture>> textures_;
	int maxFrames_ = 2;
	UniformBufferBase* ubo_;

	VkShaderModule vertShaderModule_;
	VkShaderModule fragShaderModule_;

private:
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateGraphicsPipeline();

    VkShaderModule CreateShaderModule(const std::vector<char>& shaderCode);
	void LoadShaders(std::string vertexShaderPath, std::string fragmentShaderPath);
	void AddTexture(std::shared_ptr<Texture> texture){ textures_.push_back(texture); }

	// init must call after add texture
	void Init();
};

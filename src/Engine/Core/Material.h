#pragma once
#include <vector>

#include "Core/Common.h"
#include "Core/VulkanCore.h"
#include "Core/Texture.h"

struct UniformBufferBase
{

};

struct CommonUniformBuffer
{
	glm::mat4 WVP;
	glm::mat4 VP;
	glm::mat4 W;
	glm::mat4 V;
	glm::mat4 P;
	float time;
};

struct InstancingUniformBufferBase
{
};

struct VertexData
{
	Vec3 pos;
	Vec3 color;
	Vec3 normal;
	Vec3 tangent;
	Vec3 bitangent;
	Vec2 texCoord;
};


struct MaterialInfo
{
	std::string vertShaderPath;
	std::string fragShaderPath;
	int customUiformBufferSize;
	int instancingUiformBufferSize;
	std::vector<shared_ptr<Texture>> textures;
	VkBuffer storageBuffer = VK_NULL_HANDLE;
	bool isAdditive = false;
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
		customUboBufferSize_(0),
		instancingUBO_(),
		instancingUniformBuffer_(VK_NULL_HANDLE),
		instancingUniformBufferMapped_(VK_NULL_HANDLE),
		instancingUniformBufferMemory_(VK_NULL_HANDLE),
		instancingUboBufferSize_(0),
		storageBuffer_(VK_NULL_HANDLE),
		isAdditive_(false)
	{
	}
	~Material() 
	{
		Release();
	}

	static shared_ptr<Material> Create(MaterialInfo materialInfo);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, std::vector<shared_ptr<Texture>> textures);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, int instancingUniformBufferSize,  std::vector<shared_ptr<Texture>> textures);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, std::vector<shared_ptr<Texture>> textures);
	static shared_ptr<Material> Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, int instancingUniformBufferSize,  std::vector<shared_ptr<Texture>> textures, VkBuffer storageBuffer, bool isAdditive = false);
	
	int GetUboBufferSize() { return customUboBufferSize_; }
	VkPipelineShaderStageCreateInfo* GetShaderStages() { return shaderStages_; }
	std::vector<std::shared_ptr<Texture>> GetTextures() { return textures_; }
	void SetUniformBufferData(UniformBufferBase* ubo){ ubo_ = ubo; }
	UniformBufferBase* GetUniformBuffer() { return ubo_; }
	
	int GetInstancingUboBufferSize() { return instancingUboBufferSize_; }
	void SetInstancingUniformBuffer(InstancingUniformBufferBase* ubo) { instancingUboBase_ = ubo; }
	InstancingUniformBufferBase* GetInstancingUniformBuffer() { return instancingUboBase_; }

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

	InstancingUniformBufferBase instancingUBO_;
	VkBuffer instancingUniformBuffer_;
	VkDeviceMemory instancingUniformBufferMemory_;
	void* instancingUniformBufferMapped_;

private:
	int customUboBufferSize_;
	int instancingUboBufferSize_;
	VkPipelineShaderStageCreateInfo shaderStages_[2];
	std::vector<std::shared_ptr<Texture>> textures_;
	int maxFrames_ = 2;
	UniformBufferBase* ubo_;
	InstancingUniformBufferBase* instancingUboBase_;

	VkShaderModule vertShaderModule_;
	VkShaderModule fragShaderModule_;

	VkBuffer storageBuffer_;

	bool isAdditive_;

private:
	void CreateDescriptorSetLayout();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateGraphicsPipeline();

    
	void LoadShaders(std::string vertexShaderPath, std::string fragmentShaderPath);
	void AddTexture(std::shared_ptr<Texture> texture){ textures_.push_back(texture); }

	// init must call after add texture
	void Init();

	void Release();
};

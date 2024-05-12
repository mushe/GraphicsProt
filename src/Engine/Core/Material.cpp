#include "Material.h"

#include "Core/VulkanUtil.h"
#include "IO/File.h"

void Material::Init()
{
    if(customUboBufferSize_ > 0)
        VulkanUtil::CreateBuffer(customUboBufferSize_, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer_, uniformBufferMemory_);

    if(instancingUboBufferSize_ > 0)
        VulkanUtil::CreateBuffer(instancingUboBufferSize_, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, instancingUniformBuffer_, instancingUniformBufferMemory_);

    VulkanUtil::CreateBuffer(sizeof(CommonUniformBuffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, commonUniformBuffer_, commonUniformBufferMemory_);

    CreateDescriptorSetLayout();
    CreateDescriptorPool();
    CreateDescriptorSets();
    CreateGraphicsPipeline();
}

shared_ptr<Material> Material::Create(MaterialInfo materialInfo)
{
	auto material = std::make_shared<Material>();
	material->customUboBufferSize_ = materialInfo.customUiformBufferSize;
    material->instancingUboBufferSize_ = materialInfo.instancingUiformBufferSize;

	material->LoadShaders(materialInfo.vertShaderPath, materialInfo.fragShaderPath);

    for (auto texture : materialInfo.textures)
    {
		material->AddTexture(texture);
	}
    
    material->Init();

    return material;
}

shared_ptr<Material> Material::Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, std::vector<shared_ptr<Texture>> textures)
{
    MaterialInfo materialInfo;
    materialInfo.vertShaderPath = vertShaderPath;
    materialInfo.fragShaderPath = fragShaderPath;
    materialInfo.customUiformBufferSize = uiformBufferSize;
    materialInfo.instancingUiformBufferSize = 0;
    materialInfo.textures = textures;
    return Create(materialInfo);
}

shared_ptr<Material> Material::Create(std::string vertShaderPath, std::string fragShaderPath, int uiformBufferSize, int instancingUniformBufferSize, std::vector<shared_ptr<Texture>> textures)
{
	MaterialInfo materialInfo;
	materialInfo.vertShaderPath = vertShaderPath;
	materialInfo.fragShaderPath = fragShaderPath;
	materialInfo.customUiformBufferSize = uiformBufferSize;
	materialInfo.instancingUiformBufferSize = instancingUniformBufferSize;
	materialInfo.textures = textures;
	return Create(materialInfo);
}

shared_ptr<Material> Material::Create(std::string vertShaderPath, std::string fragShaderPath, std::vector<shared_ptr<Texture>> textures)
{
    MaterialInfo materialInfo;
    materialInfo.vertShaderPath = vertShaderPath;
    materialInfo.fragShaderPath = fragShaderPath;
    materialInfo.customUiformBufferSize = 0;
    materialInfo.instancingUiformBufferSize = 0;
    materialInfo.textures = textures;
    return Create(materialInfo);
}

void Material::LoadShaders(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    // xxxx.vert -> xxxxVert.spv
    std::string shaderDirectory = "../Shaders/spv/";
    vertexShaderPath = shaderDirectory + vertexShaderPath.erase(vertexShaderPath.length() - 5) + "Vert.spv";
    fragmentShaderPath = shaderDirectory + fragmentShaderPath.erase(fragmentShaderPath.length() - 5) + "Frag.spv";


    auto vertShaderCode = File::ReadShaderFile(vertexShaderPath);
    auto fragShaderCode = File::ReadShaderFile(fragmentShaderPath);

    vertShaderModule_ = VulkanUtil::CreateShaderModule(vertShaderCode);
    fragShaderModule_ = VulkanUtil::CreateShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule_;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule_;
    fragShaderStageInfo.pName = "main";

    shaderStages_[0] = vertShaderStageInfo;
    shaderStages_[1] = fragShaderStageInfo;
}

void Material::CreateDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings.push_back(uboLayoutBinding);

    VkDescriptorSetLayoutBinding commonUboLayoutBinding{};
    commonUboLayoutBinding.binding = 1;
    commonUboLayoutBinding.descriptorCount = 1;
    commonUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    commonUboLayoutBinding.pImmutableSamplers = nullptr;
    commonUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings.push_back(commonUboLayoutBinding);

    VkDescriptorSetLayoutBinding instancingUboLayoutBinding{};
    instancingUboLayoutBinding.binding = 2;
    instancingUboLayoutBinding.descriptorCount = 1;
    instancingUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    instancingUboLayoutBinding.pImmutableSamplers = nullptr;
    instancingUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings.push_back(instancingUboLayoutBinding);

    for (int i = 0; i < textures_.size(); i++)
    {
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 3 + i;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings.push_back(samplerLayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    assert(vkCreateDescriptorSetLayout(VulkanCore::GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout_) == VK_SUCCESS);
}

void Material::CreateDescriptorPool()
{
    std::vector<VkDescriptorPoolSize> poolSizes;

    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(maxFrames_);
    poolSizes.push_back(poolSize);

    VkDescriptorPoolSize commonPoolSize{};
    commonPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    commonPoolSize.descriptorCount = static_cast<uint32_t>(maxFrames_);
    poolSizes.push_back(commonPoolSize);

    VkDescriptorPoolSize instancingPoolSize{};
    instancingPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    instancingPoolSize.descriptorCount = static_cast<uint32_t>(maxFrames_);
    poolSizes.push_back(instancingPoolSize);

    for (auto dummy : textures_)
    {
        VkDescriptorPoolSize imagePoolSize{};
        imagePoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        imagePoolSize.descriptorCount = static_cast<uint32_t>(maxFrames_);
        poolSizes.push_back(imagePoolSize);
    }

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(maxFrames_);

    assert(vkCreateDescriptorPool(VulkanCore::GetDevice(), &poolInfo, nullptr, &descriptorPool_) == VK_SUCCESS);
}

void Material::CreateDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(maxFrames_, descriptorSetLayout_);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool_;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(maxFrames_);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets_.resize(maxFrames_);
    assert(vkAllocateDescriptorSets(VulkanCore::GetDevice(), &allocInfo, descriptorSets_.data()) == VK_SUCCESS);

    for (size_t i = 0; i < maxFrames_; i++)
    {
        std::vector<VkWriteDescriptorSet> descriptorWrites;

        if (customUboBufferSize_ > 0)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer_;
            bufferInfo.offset = 0;
            bufferInfo.range = customUboBufferSize_;
            VkWriteDescriptorSet uboDescriptorWrite{};
            uboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboDescriptorWrite.dstSet = descriptorSets_[i];
            uboDescriptorWrite.dstBinding = 0;
            uboDescriptorWrite.dstArrayElement = 0;
            uboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboDescriptorWrite.descriptorCount = 1;
            uboDescriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrites.push_back(uboDescriptorWrite);
        }

        VkDescriptorBufferInfo commonBufferInfo{};
        commonBufferInfo.buffer = commonUniformBuffer_;
        commonBufferInfo.offset = 0;
        commonBufferInfo.range = sizeof(CommonUniformBuffer);
        VkWriteDescriptorSet commonUboDescriptorWrite{};
        commonUboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        commonUboDescriptorWrite.dstSet = descriptorSets_[i];
        commonUboDescriptorWrite.dstBinding = 1;
        commonUboDescriptorWrite.dstArrayElement = 0;
        commonUboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        commonUboDescriptorWrite.descriptorCount = 1;
        commonUboDescriptorWrite.pBufferInfo = &commonBufferInfo;
        descriptorWrites.push_back(commonUboDescriptorWrite);

        if (instancingUboBufferSize_ > 0)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = instancingUniformBuffer_;
            bufferInfo.offset = 0;
            bufferInfo.range = instancingUboBufferSize_;
            VkWriteDescriptorSet uboDescriptorWrite{};
            uboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboDescriptorWrite.dstSet = descriptorSets_[i];
            uboDescriptorWrite.dstBinding = 2;
            uboDescriptorWrite.dstArrayElement = 0;
            uboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            uboDescriptorWrite.descriptorCount = 1;
            uboDescriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrites.push_back(uboDescriptorWrite);
        }

        std::vector<VkDescriptorImageInfo> imageInfos;
        for (int j = 0; j < textures_.size(); j++)
        {
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = *textures_[j]->GetImageView();
            imageInfo.sampler = *textures_[j]->GetSampler();
            imageInfos.push_back(imageInfo);
        }

        for (int j = 0; j < textures_.size(); j++)
        {
            VkWriteDescriptorSet textureDescriptorWrite{};
            textureDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            textureDescriptorWrite.dstSet = descriptorSets_[i];
            textureDescriptorWrite.dstBinding = j + 3;
            textureDescriptorWrite.dstArrayElement = 0;
            textureDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            textureDescriptorWrite.descriptorCount = 1;
            textureDescriptorWrite.pImageInfo = &imageInfos[j];
            descriptorWrites.push_back(textureDescriptorWrite);
        }

        vkUpdateDescriptorSets(VulkanCore::GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}



void Material::CreateGraphicsPipeline()
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    std::array<VkVertexInputAttributeDescription, 6> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(VertexData, pos);
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(VertexData, color);
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(VertexData, normal);
    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(VertexData, tangent);
    attributeDescriptions[4].binding = 0;
    attributeDescriptions[4].location = 4;
    attributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[4].offset = offsetof(VertexData, bitangent);
    attributeDescriptions[5].binding = 0;
    attributeDescriptions[5].location = 5;
    attributeDescriptions[5].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[5].offset = offsetof(VertexData, texCoord);

    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(VertexData);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    //rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;

    assert(vkCreatePipelineLayout(VulkanCore::GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) == VK_SUCCESS);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages_;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout_;
    pipelineInfo.renderPass = VulkanCore::GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    assert(vkCreateGraphicsPipelines(VulkanCore::GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_) == VK_SUCCESS);
}


void Material::Release()
{
    vkDestroyPipeline(VulkanCore::GetDevice(), pipeline_, nullptr);
    vkDestroyPipelineLayout(VulkanCore::GetDevice(), pipelineLayout_, nullptr);
    vkDestroyDescriptorPool(VulkanCore::GetDevice(), descriptorPool_, nullptr);
    vkDestroyDescriptorSetLayout(VulkanCore::GetDevice(), descriptorSetLayout_, nullptr);

    if (customUboBufferSize_ > 0)
    {
        vkDestroyBuffer(VulkanCore::GetDevice(), uniformBuffer_, nullptr);
        vkFreeMemory(VulkanCore::GetDevice(), uniformBufferMemory_, nullptr);
    }

    if (instancingUboBufferSize_ > 0)
    {
        vkDestroyBuffer(VulkanCore::GetDevice(), instancingUniformBuffer_, nullptr);
        vkFreeMemory(VulkanCore::GetDevice(), instancingUniformBufferMemory_, nullptr);
    }

    vkDestroyBuffer(VulkanCore::GetDevice(), commonUniformBuffer_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), commonUniformBufferMemory_, nullptr);

    vkDestroyShaderModule(VulkanCore::GetDevice(), fragShaderModule_, nullptr);
    vkDestroyShaderModule(VulkanCore::GetDevice(), vertShaderModule_, nullptr);
}
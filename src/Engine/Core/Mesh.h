#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Material.h"
#include <glm/glm.hpp>

#include "Core/VulkanCore.h"
#include "Core/Transform.h"
#include "Core/Camera.h"
#include "IO/ObjParser.h"
#include "Utility/Debug.h"

class Mesh
{

private:

public:
    Mesh() :
        transform_({Vec3(0), Vec3(0,1,0), Vec3(1)}),
        vertexBuffer_(VK_NULL_HANDLE),
        vertexBufferMemory_(VK_NULL_HANDLE),
        indexBuffer_(VK_NULL_HANDLE),
        indexBufferMemory_(VK_NULL_HANDLE),
        material_(nullptr)
    {
    }

    static std::shared_ptr<Mesh> Create();
    static std::shared_ptr<Mesh> FromOBJ(std::string objFilePath, int face = 3);

    ~Mesh()
    {
        Release();
    }


    void Draw(const Camera& camera, const int instanceCount = 1);


    static std::shared_ptr<Mesh> LoadFromObjFile(std::string filePath)
    {
        return std::make_shared<Mesh>();
    }
    

    void SetMaterial(std::shared_ptr<Material> material) { material_ = material.get(); }
    
    void SetVertices(vector<VertexData> vertices) { vertices_ = vertices; }
    void SetIndices(vector<uint32_t> indices) { indices_ = indices; }

    void Init();

    void SetPosition(Vec3 position) { transform_.position = position; }
    void SetRotation(Vec3 rotation) { transform_.rotation = rotation; }
    void SetScale(Vec3 scale) { transform_.scale = scale; }
    Transform transform_;


private:
    
    void CreateVertexBuffer();
    void CreateIndexBuffer();
    void Release();

private:
    std::vector<VertexData> vertices_;
    std::vector<uint32_t> indices_;

    VkBuffer vertexBuffer_;
    VkDeviceMemory vertexBufferMemory_;
    VkBuffer indexBuffer_;
    VkDeviceMemory indexBufferMemory_;

    const int maxFrames_ = 2;

    Material* material_;

    float time_ = 0.0f;
};

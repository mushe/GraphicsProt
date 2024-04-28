#pragma once
#include "Core/Scene.h"

struct NormalMapUBO :public UniformBufferBase
{
    Vec3 lightDir = Vec3(0.85f, 2.2f, 3.67f);
    float diffuseIntensity = 0.12f;

    Vec3 baseColor = Vec3(0.0f, 0.0f, 0.0f);
    float diffusePower = 1.0f;

    Vec3 cameraPos = Vec3(0.0f, 0.0, 0.0);
    float specularIntensity = 0.48f;

    float specularPower = 5.6f;
};

class NormalMapping : public Scene
{
public:
    NormalMapping():
        meshRotation_(0, 0, 0)
    {
        Debug::Log("NormalMapping()");
    }
    virtual ~NormalMapping() { Debug::Log("~NormalMapping()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> floorMesh_;
    Camera camera_;
    shared_ptr<Texture> floorTex_;
    shared_ptr<Material> floorMat_;
    NormalMapUBO ubo_;
    Vec3 meshRotation_;
};
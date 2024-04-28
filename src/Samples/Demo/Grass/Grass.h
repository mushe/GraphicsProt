#pragma once
#include "Core/Scene.h"

struct GrassUBO :public UniformBufferBase
{
    float noiseSpeed = 0.5f;
    float noiseScale = 1.8f;
    float noiseTimeOffset = 0.0f;
};

class Grass : public Scene
{
public:
    Grass():
        noiseScale_(0.96f),
        noiseSpeed_(0.63f)
    {
        Debug::Log("Grass()");
    }
    virtual ~Grass() { Debug::Log("~Grass()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> floorMesh_;
    shared_ptr<Texture> floorTex_;
    shared_ptr<Material> floorMat_;
    vector<shared_ptr<Mesh>> meshes_;
    vector<shared_ptr<Material>> materials_;
    vector<shared_ptr<GrassUBO>> ubos_;
    float noiseScale_;
    float noiseSpeed_;
    Camera camera_;
};
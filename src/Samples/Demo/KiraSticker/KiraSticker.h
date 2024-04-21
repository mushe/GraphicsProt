#pragma once
#include "Core/Scene.h"

struct KiraUBO :public UniformBufferBase
{
    float division = 10.0f;
    float scrollSpeed = 0.2f;
};

class KiraSticker : public Scene
{
public:
    KiraSticker()
    {
        Debug::Log("KiraSticker()");
    }
    virtual ~KiraSticker() { Debug::Log("~KiraSticker()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> floorMesh_;
    shared_ptr<Texture> gradTex_;
    shared_ptr<Material> floorMat_;
    KiraUBO ubo_;
    Camera camera_;
};
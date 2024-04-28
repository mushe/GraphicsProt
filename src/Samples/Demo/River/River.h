#pragma once
#include "Core/Scene.h"

struct RiverUBO :public UniformBufferBase
{
    float distortionSpeed = 0.0f;
    float distortionScale = 0.0f;
    float distortionPosScale = 0.0f;
    float noiseScaleX_1 = 0.0f;

    float noiseScaleY_1 = 0.0f;
    float noiseSpeed_1 = 0.0f;
    float noiseScaleX_2 = 0.0f;
    float noiseScaleY_2 = 0.0f;

    float noiseSpeed_2 = 0.0f;
    float noiseThreshold = 0.0f;
    float noisePower = 0.0f;
    float noiseMultiply = 0.0f;

    float colorLerp = 0.0f;
    float riverColR = 0.0f;
    float riverColG = 0.0f;
    float riverColB = 0.0f;
};

class River : public Scene
{
public:
    River():
        colorLerp_(0.37f),
        distortionPosScale_(90.0f),
        distortionScale_(0.005f),
        distortionSpeed_(5.57f),
        noiseMultiply_(14.56f),
        noisePower_(4.12f),
        noiseScaleX_1_(6.03f),
        noiseScaleX_2_(3.43f),
        noiseScaleY_1_(18.53f),
        noiseScaleY_2_(10.64f),
        noiseSpeed_1_(2.43f),
        noiseSpeed_2_(2.8f),
        noiseThreshold_(0.27f),
        riverColR_(0.194f),
        riverColG_(0.26f),
        riverColB_(0.413f)
    {
        Debug::Log("River()");
    }
    virtual ~River() { Debug::Log("~River()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:

    float colorLerp_;
    float distortionPosScale_;
    float distortionScale_;
    float distortionSpeed_;
    float noiseMultiply_;
    float noisePower_;
    float noiseScaleX_1_;
    float noiseScaleX_2_;
    float noiseScaleY_1_;
    float noiseScaleY_2_;
    float noiseSpeed_1_;
    float noiseSpeed_2_;
    float noiseThreshold_;
    float riverColB_;
    float riverColG_;
    float riverColR_;

    shared_ptr<Mesh> mesh_;
    shared_ptr<Texture> tex1_;
    shared_ptr<Texture> tex2_;
    Camera camera_;
    RiverUBO ubo_;
    shared_ptr<Material> mat_;
    Vec3 meshRotation_;
};
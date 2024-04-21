#pragma once
#include "Core/Scene.h"

struct GerstnerWaveUBO :public UniformBufferBase
{
    Vec3 waveColor = Vec3(0.016f, 0.027f, 0.067f);
    float diffusePower = 1.0f;

    Vec3 lightDirection = Vec3(3.93f, 0.96f, -0.37f);
    float diffuseMultiply = 0.068f;

    Vec3 lightColor = Vec3(1.0f, 0.74f, 0.22f);
    float specularPower = 30.29f;

    Vec3 cameraPos = Vec3(1);
    float specularMultiply = 0.118f;

    float sunSpread = 0.14f;
    float sunRefrectionThreshold = -0.01f;
    float dummy1 = 1.0f;
    float dummy2 = 1.0f;

    Vec3 sunColor = Vec3(0.60f, 0.32f, 0.0f);
    float sunIntensity = 16.0f;

    float waveHeight_1 = 0.001f;
    float waveHeight_2 = 0.002f;
    float waveHeight_3 = 0.002f;
    float waveHeight_4 = 0.004f;

    float waveHeight_5 = 0.013f;
    float wavePosScale_1 = 226.3f;
    float wavePosScale_2 = 208.6f;
    float wavePosScale_3 = 127.8f;

    float wavePosScale_4 = 122.1f;
    float wavePosScale_5 = 12.6f;
    float waveSpeed_1 = 1.97f;
    float waveSpeed_2 = 0.35f;

    float waveSpeed_3 = 0.77f;
    float waveSpeed_4 = 7.78f;
    float waveSpeed_5 = 3.24f;
    float dummy = 1.0f;

    Vec2 waveDirection_1 = Vec2(1.0f, 0.24f);
    Vec2 waveDirection_2 = Vec2(0.82f, 0.97f);
    Vec2 waveDirection_3 = Vec2(1.0f, -1.62f);
    Vec2 waveDirection_4 = Vec2(1.23f, 3.52f);
    Vec2 waveDirection_5 = Vec2(3.13f, 2.80f);
};

class GerstnerWaves : public Scene
{
public:
    GerstnerWaves()
    {
        Debug::Log("GerstnerWaves()");
    }
    virtual ~GerstnerWaves(){Debug::Log("~GerstnerWaves()");}

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> waveMesh_;
    shared_ptr<Material> waveMat;
    shared_ptr<Mesh> bgMesh;
    shared_ptr<Material> bgMat;
    GerstnerWaveUBO waveUBO{};
    Camera camera;
};
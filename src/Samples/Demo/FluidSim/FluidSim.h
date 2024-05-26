#pragma once
#include "Core/Scene.h"

#include "FluidComputeShader.h"

struct FluidRenderUBO : InstancingUniformBufferBase
{
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec3 vel;
    float density;
    float pressure;
};

struct FluidCustomUBO :public UniformBufferBase
{
    float scale = 5.0f;
};

struct FluidCompositeUBO :public UniformBufferBase
{
    alignas(16) Vec4 ColorA = Vec4(0.094f, 0.431f, 1.0f, 0.0f);
    alignas(16) Vec4 ColorB = Vec4(0.094f, 1.0f, 0.812f, 0.0f);
    float diffusePower = 4.5f;
    float diffuseMultiply = 0.03f;
    float specularPower = 8.0f;
    float specularMultiply = 0.0008f;
    float floorColorBlendPower = 0.336f;
    float floorColorBlendMultiply = 1.83f; 
};

struct FluidDepthProcessUBO :public UniformBufferBase
{
    float scale = 3.3f;
};

struct FluidBlurPostProcessUBO :public UniformBufferBase
{
    Vec2 resolution = Vec2(2048, 2048);
    float scale = 1.2f;
};

class FluidSim : public Scene
{
public:
    FluidSim():
        desiredParticleCount_(10000),
        particleCount_(desiredParticleCount_ - desiredParticleCount_ % 256),
        fieldScale_(3.0f),
        fullRes_(2048, 2048),
        res_1_2_(fullRes_.x/2.0f, fullRes_.y/2.0f),
        res_1_4_(fullRes_.x/4.0f, fullRes_.y/4.0f),
        res_1_8_(fullRes_.x/8.0f, fullRes_.y/8.0f),
        res_1_16_(fullRes_.x/16.0f, fullRes_.y/16.0f),
        blurScale_(0.904f),
        reserveSceneClose_(false),
        reserveSceneCloseWaitFrame_(0)
    {
    }
    virtual ~FluidSim() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

    void InitBlurForDepth();
    void InitBlurForColor();


private:
    void ResetSimulation();

    FluidComputeShader computeShader_;
    uint32_t desiredParticleCount_;
    uint32_t particleCount_;
    float fieldScale_;
    vector<FluidRenderUBO> computeUBO_;

    // shareing buffer between compute shader and instancing shader
    VkBuffer sharingBuffer_;
    VkDeviceMemory sharingBufferMemory_;

    shared_ptr<RenderTarget> rt_;
    shared_ptr<Material> postProcessMat_;
    FluidCompositeUBO ubo_;

    shared_ptr<RenderTarget> rt_depth_processed_;
    shared_ptr<Material> depthProcessMat_;


    Camera camera_;

    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    FluidCustomUBO customUbo_;

    shared_ptr<RenderTarget> rt_fluid_additive_;
    shared_ptr<Material> mat_fluid_additive_;
    shared_ptr<Mesh> mesh_fluid_additive_;


    // blur
    float blurScale_;
    Vec2 fullRes_;
    Vec2 res_1_2_;
    Vec2 res_1_4_;
    Vec2 res_1_8_;
    Vec2 res_1_16_;

    shared_ptr<RenderTarget> rt_1_2_down_;
    shared_ptr<RenderTarget> rt_1_4_down_;
    shared_ptr<RenderTarget> rt_1_8_down_;
    shared_ptr<RenderTarget> rt_1_16_down_;
    shared_ptr<RenderTarget> rt_1_8_up_;
    shared_ptr<RenderTarget> rt_1_4_up_;
    shared_ptr<RenderTarget> rt_1_2_up_;
    shared_ptr<RenderTarget> rt_1_1_up_;

    shared_ptr<Material> mat_1_2_down_;
    shared_ptr<Material> mat_1_4_down_;
    shared_ptr<Material> mat_1_8_down_;
    shared_ptr<Material> mat_1_16_down_;
    shared_ptr<Material> mat_1_8_up_;
    shared_ptr<Material> mat_1_4_up_;
    shared_ptr<Material> mat_1_2_up_;
    shared_ptr<Material> mat_1_1_up_;

    FluidBlurPostProcessUBO ubo_1_2_down_;
    FluidBlurPostProcessUBO ubo_1_4_down_;
    FluidBlurPostProcessUBO ubo_1_8_down_;
    FluidBlurPostProcessUBO ubo_1_16_down_;
    FluidBlurPostProcessUBO ubo_1_8_up_;
    FluidBlurPostProcessUBO ubo_1_4_up_;
    FluidBlurPostProcessUBO ubo_1_2_up_;
    FluidBlurPostProcessUBO ubo_1_1_up_;

    shared_ptr<RenderTarget> rt_1_2_down_color_;
    shared_ptr<RenderTarget> rt_1_4_down_color_;
    shared_ptr<RenderTarget> rt_1_8_down_color_;
    shared_ptr<RenderTarget> rt_1_16_down_color_;
    shared_ptr<RenderTarget> rt_1_8_up_color_;
    shared_ptr<RenderTarget> rt_1_4_up_color_;
    shared_ptr<RenderTarget> rt_1_2_up_color_;
    shared_ptr<RenderTarget> rt_1_1_up_color_;

    shared_ptr<Material> mat_1_2_down_color_;
    shared_ptr<Material> mat_1_4_down_color_;
    shared_ptr<Material> mat_1_8_down_color_;
    shared_ptr<Material> mat_1_16_down_color_;
    shared_ptr<Material> mat_1_8_up_color_;
    shared_ptr<Material> mat_1_4_up_color_;
    shared_ptr<Material> mat_1_2_up_color_;
    shared_ptr<Material> mat_1_1_up_color_;


    shared_ptr<Mesh> floorMesh_;
    shared_ptr<Texture> floorTex_;
    shared_ptr<Material> floorMat_;
    shared_ptr<RenderTarget> rt_floor_;

    bool reserveSceneClose_;
    int reserveSceneCloseWaitFrame_;

};
#pragma once
#include "Core/Scene.h"

struct BlurPostProcessUBO :public UniformBufferBase
{
    Vec2 resolution = Vec2(2048, 2048);
    float scale = 2.5f;
};

class Blur : public Scene
{
public:
    Blur():
        fullRes_(2048, 2048),
        res_1_2_(fullRes_.x/2.0f, fullRes_.y/2.0f),
        res_1_4_(fullRes_.x/4.0f, fullRes_.y/4.0f),
        res_1_8_(fullRes_.x/8.0f, fullRes_.y/8.0f),
        res_1_16_(fullRes_.x/16.0f, fullRes_.y/16.0f),
        blurScale_(2.5f),
        elapsedTime_(0.0f)
    { }
    virtual ~Blur(){ }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> mesh_;
    shared_ptr<Material> mat_;
    shared_ptr<Texture> tex_;
    shared_ptr<Material> postProcessMat_;
    shared_ptr<RenderTarget> rt_;
    Camera camera_;
    
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

    BlurPostProcessUBO ubo_1_2_down_;
    BlurPostProcessUBO ubo_1_4_down_;
    BlurPostProcessUBO ubo_1_8_down_;
    BlurPostProcessUBO ubo_1_16_down_;
    BlurPostProcessUBO ubo_1_8_up_;
    BlurPostProcessUBO ubo_1_4_up_;
    BlurPostProcessUBO ubo_1_2_up_;
    BlurPostProcessUBO ubo_1_1_up_;

    float blurScale_;
    float elapsedTime_;
};
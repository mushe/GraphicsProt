#include "Blur.h"

void Blur::Start(shared_ptr<Engine> engine)
{
    // mesh
    mesh_ = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh_->SetPosition(Vec3(0, -1.0f, 0));
    mesh_->SetRotation(Vec3(0));
    mesh_->SetScale(Vec3(0.1f));
    tex_ = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");
    mat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { tex_ });
    mesh_->SetMaterial(mat_);


    rt_ = RenderTarget::Create(fullRes_.x, fullRes_.y);
    rt_1_2_down_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_4_down_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_8_down_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_16_down_ = RenderTarget::Create(res_1_16_.x, res_1_16_.y);
    rt_1_8_up_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_4_up_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_2_up_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_1_up_ = RenderTarget::Create(fullRes_.x, fullRes_.y);


    mat_1_2_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(BlurPostProcessUBO),
        { rt_->GetColorTexture() }
    );
    ubo_1_2_down_.resolution = res_1_2_;
    mat_1_2_down_->SetUniformBufferData(&ubo_1_2_down_);

    mat_1_4_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_2_down_->GetColorTexture() }
    );
    ubo_1_4_down_.resolution = res_1_4_;
    mat_1_4_down_->SetUniformBufferData(&ubo_1_4_down_);

    mat_1_8_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_4_down_->GetColorTexture() }
    );
    ubo_1_8_down_.resolution = res_1_8_;
    mat_1_8_down_->SetUniformBufferData(&ubo_1_8_down_);


    mat_1_16_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_8_down_->GetColorTexture() }
    );
    ubo_1_16_down_.resolution = res_1_16_;
    mat_1_16_down_->SetUniformBufferData(&ubo_1_16_down_);


    mat_1_8_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_16_down_->GetColorTexture() }
    );
    ubo_1_8_up_.resolution = res_1_8_;
    mat_1_8_up_->SetUniformBufferData(&ubo_1_8_up_);


    mat_1_4_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_8_up_->GetColorTexture() }
    );
    ubo_1_4_up_.resolution = res_1_4_;
    mat_1_4_up_->SetUniformBufferData(&ubo_1_4_up_);


    mat_1_2_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_4_up_->GetColorTexture() }
    );
    ubo_1_2_up_.resolution = res_1_2_;
    mat_1_2_up_->SetUniformBufferData(&ubo_1_2_up_);


    mat_1_1_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(BlurPostProcessUBO),
        { rt_1_2_up_->GetColorTexture() }
    );
    ubo_1_1_up_.resolution = fullRes_;
    mat_1_1_up_->SetUniformBufferData(&ubo_1_1_up_);



    OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
}


bool Blur::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    elapsedTime_ += 0.016f;
    blurScale_ = 3.0f + sin(elapsedTime_) * 2.5f;

    bool closeScene = false;

    OrbitalControl::Update(camera_);

    // object rendering to render target
    rt_->BeginRenderToTexture();
    mesh_->Draw(camera_);
    rt_->EndRenderToTexture();

    engine->BlitToRenderTarget(mat_1_2_down_, rt_1_2_down_);
    engine->BlitToRenderTarget(mat_1_4_down_, rt_1_4_down_);
    engine->BlitToRenderTarget(mat_1_8_down_, rt_1_8_down_);
    engine->BlitToRenderTarget(mat_1_16_down_, rt_1_16_down_);
    engine->BlitToRenderTarget(mat_1_8_up_, rt_1_8_up_);
    engine->BlitToRenderTarget(mat_1_4_up_, rt_1_4_up_);
    engine->BlitToRenderTarget(mat_1_2_up_, rt_1_2_up_);

    // post process rendering to screen
    engine->BeginRenderToScreen();
    engine->BlitToScreen(mat_1_1_up_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
        GUI::FloatSlider(blurScale_, "blur scale", 0.7f, 8.0f);
    });

    ubo_1_2_down_.scale = blurScale_;
    ubo_1_4_down_.scale = blurScale_;
    ubo_1_8_down_.scale = blurScale_;
    ubo_1_16_down_.scale = blurScale_;
    ubo_1_8_up_.scale = blurScale_;
    ubo_1_4_up_.scale = blurScale_;
    ubo_1_2_up_.scale = blurScale_;
    ubo_1_1_up_.scale = blurScale_;

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Blur::Release()
{
    mesh_ = nullptr;
    mat_ = nullptr;
    tex_ = nullptr;
    postProcessMat_ = nullptr;
    rt_ = nullptr;

    rt_1_2_down_ = nullptr;
    rt_1_4_down_ = nullptr;
    rt_1_8_down_ = nullptr;
    rt_1_16_down_ = nullptr;
    rt_1_8_up_ = nullptr;
    rt_1_4_up_ = nullptr;
    rt_1_2_up_ = nullptr;
    rt_1_1_up_ = nullptr;

    mat_1_2_down_ = nullptr;
    mat_1_4_down_ = nullptr;
    mat_1_8_down_ = nullptr;
    mat_1_16_down_ = nullptr;
    mat_1_8_up_ = nullptr;
    mat_1_4_up_ = nullptr;
    mat_1_2_up_ = nullptr;
    mat_1_1_up_ = nullptr;
}

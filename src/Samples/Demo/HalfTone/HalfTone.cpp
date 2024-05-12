#include "HalfTone.h"


void HalfTone::Start(shared_ptr<Engine> engine)
{
    mesh_ = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh_->SetPosition(Vec3(0, -1.0f, 0));
    mesh_->SetRotation(Vec3(0));
    mesh_->SetScale(Vec3(0.1f));
    tex_ = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");
    mat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { tex_ });
    mesh_->SetMaterial(mat_);

    // post process
    rt_ = RenderTarget::Create(1920, 1080);
    postProcessMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_HalfTone.frag",
        sizeof(HalfTonePostProcessUBO),
        { rt_->GetColorTexture() }
    );
    postProcessMat_->SetUniformBufferData(&ubo_);

    GUI::ChangeStyleToLight();
}


bool HalfTone::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();
    OrbitalControl::Update(camera_);

    bool closeScene = false;


    // object rendering to render target
    rt_->BeginRenderToTexture();
    mesh_->Draw(camera_);
    rt_->EndRenderToTexture();

    // post process rendering to screen
    engine->BeginRenderToScreen();
    engine->BlitToScreen(postProcessMat_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void HalfTone::Release()
{
    mesh_ = nullptr;
    mat_ = nullptr;
    tex_ = nullptr;
    postProcessMat_ = nullptr;
    rt_ = nullptr;
}

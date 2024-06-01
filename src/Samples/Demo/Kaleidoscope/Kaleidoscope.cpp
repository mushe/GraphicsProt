#include "Kaleidoscope.h"


void Kaleidoscope::Start(shared_ptr<Engine> engine)
{
    // mesh
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
        "renderToScreen.vert", "PP_Kaleidoscope.frag",
        sizeof(KaleidoscopePostProcessUBO),
        { rt_->GetColorTexture() }
    );
    postProcessMat_->SetUniformBufferData(&ubo_);
    OrbitalControl::SetOrbit(0.413713f, 6.743663f, 5.470689f);
}


bool Kaleidoscope::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    t_ += 0.016f;
    OrbitalControl::Update(camera_);
    OrbitalControl::SetRadius(5.470689f + sin(t_) * 5.0f);

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
        GUI::Float(ubo_.division, "division", 1.0f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();

    if (closeScene) 
    { 
        OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
        OrbitalControl::Update(camera_);
    }
    return closeScene;
}


void Kaleidoscope::Release()
{
    mesh_ = nullptr;
    mat_ = nullptr;
    tex_ = nullptr;
    postProcessMat_ = nullptr;
    rt_ = nullptr;
}

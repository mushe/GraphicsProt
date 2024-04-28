#include "ChromaticAberration.h"


void ChromaticAberration::Start(shared_ptr<Engine> engine)
{
    Debug::Log("ChromaticAberration::Start()");

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
        "renderToScreen.vert", "PP_ChromaticAberration.frag",
        sizeof(ChromaticAberrationPostProcessUBO),
        { rt_->GetColorTexture() }
    );
    postProcessMat_->SetUniformBufferData(&ubo_);
    OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
}


bool ChromaticAberration::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    OrbitalControl::Update(camera_);

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
        GUI::FloatSlider(ubo_.distance, "distance", 0.0f, 0.02f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void ChromaticAberration::Release()
{
    Debug::Log("ChromaticAberration::Release()");
}

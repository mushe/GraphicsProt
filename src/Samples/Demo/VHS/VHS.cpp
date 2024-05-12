#include "VHS.h"


void VHS::Start(shared_ptr<Engine> engine)
{
    // mesh
    mesh_ = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh_->SetPosition(Vec3(0, -1.0f, 0));
    mesh_->SetRotation(Vec3(0));
    mesh_->SetScale(Vec3(0.1f));
    tex_ = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");
    mat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { tex_ });
    mesh_->SetMaterial(mat_);

    // post process 1
    rt1_ = RenderTarget::Create(1920, 1080);
    chromaticAberrationMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_ChromaticAberration.frag",
        sizeof(ChromaticAberrationUBO),
        { rt1_->GetColorTexture() }
    );
    chromaticAberrationMat_->SetUniformBufferData(&chromaUbo_);

    // post process 2
    rt2_ = RenderTarget::Create(1920, 1080);
    vhsMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_VHS.frag",
        sizeof(VhsUBO),
        { rt2_->GetColorTexture() }
    );
    vhsMat_->SetUniformBufferData(&vhsUbo_);

    OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
}


bool VHS::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    OrbitalControl::Update(camera_);

    // object rendering to render target
    rt1_->BeginRenderToTexture();
    mesh_->Draw(camera_);
    rt1_->EndRenderToTexture();
    engine->BlitToRenderTarget(chromaticAberrationMat_, rt2_);

    // post process rendering to screen
    engine->BeginRenderToScreen();
    engine->BlitToScreen(vhsMat_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
        GUI::FloatSlider(chromaUbo_.distance, "chroma distance", 0.0f, 0.025f);
        GUI::FloatSlider(vhsUbo_.scale, "scale", 0.0f, 15.0f);
        GUI::Float(vhsUbo_.leapThreshold, "leapThreshold", 0.01f);
        GUI::Float(vhsUbo_.leapScale, "leapScale", 0.01f);
        GUI::Float(vhsUbo_.leapSpeed, "leapSpeed", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void VHS::Release()
{
    mesh_ = nullptr;
    mat_ = nullptr;
    tex_ = nullptr;
    chromaticAberrationMat_ = nullptr;
    vhsMat_ = nullptr;
    rt1_ = nullptr;
    rt2_ = nullptr;
}

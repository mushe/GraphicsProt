#include "Minimum.h"


void Minimum::Start(shared_ptr<Engine> engine)
{
    // create mesh from obj
    mesh_ = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh_->SetPosition(Vec3(0, -1.0f, 0));
    mesh_->SetRotation(Vec3(0));
    mesh_->SetScale(Vec3(0.1f));

    // load texture
    tex_ = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");

    // create material by shader and texture
    mat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { tex_ });

    // set material to mesh
    mesh_->SetMaterial(mat_);

    OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
}


bool Minimum::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    // camera orbital control with mouse
    OrbitalControl::Update(camera_);

    // object rendering to screen by camera
    engine->BeginRenderToScreen();
    mesh_->Draw(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Minimum::Release()
{
    mesh_ = nullptr;
    mat_ = nullptr;
    tex_ = nullptr;
}

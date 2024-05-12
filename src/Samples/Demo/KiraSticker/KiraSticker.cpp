#include "KiraSticker.h"


void KiraSticker::Start(shared_ptr<Engine> engine)
{
    floorMesh_ = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    floorMesh_->SetIndices(plane.indices);
    floorMesh_->SetVertices(plane.vertices);
    floorMesh_->Init();
    floorMesh_->SetRotation(Vec3(0, 0, 0));
    floorMesh_->SetScale(Vec3(10.0f));
    gradTex_ = Texture::FromPNG("../Textures/KiraGrad.png");
    floorMat_ = Material::Create
    (
        "Standard.vert", "KiraSticker.frag",
        sizeof(KiraUBO),
        { gradTex_ }
    );
    floorMat_->SetUniformBufferData(&ubo_);
    floorMesh_->SetMaterial(floorMat_);

    OrbitalControl::SetOrbit(0.0f, 0.0f, 12.0f);
    OrbitalControl::Update(camera_);

    GUI::ChangeStyleToLight();
}


bool KiraSticker::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    OrbitalControl::SetOrbit(0.0f, 0.0f, 12.0f);
    OrbitalControl::Update(camera_);

    engine->BeginRenderToScreen();
    floorMesh_->Draw(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
        GUI::Float(ubo_.division, "division", 1.00f);
        GUI::Float(ubo_.scrollSpeed, "scrollSpeed", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();

    return closeScene;
}


void KiraSticker::Release()
{
    floorMesh_ = nullptr;
    gradTex_ = nullptr;
    floorMat_ = nullptr;
}

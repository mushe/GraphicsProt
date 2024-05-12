#include "Posterization.h"


void Posterization::Start(shared_ptr<Engine> engine)
{
    tex_ = Texture::FromPNG("../Textures/mountain.png");
    auto plane = ModelGenerator::GeneratePlane(100);
    colorMesh_ = Mesh::Create();
    colorMesh_->SetIndices(plane.indices);
    colorMesh_->SetVertices(plane.vertices);
    colorMesh_->Init();
    colorMesh_->SetPosition(Vec3(5, 0, 0));
    colorMesh_->SetRotation(Vec3(0, 0, 0));
    colorMesh_->SetScale(Vec3(10.0f));
    colorMat_ = Material::Create
    (
        "Standard.vert", "Posterization.frag",
        sizeof(PosterizationUBO),
        { tex_ }
    );
    colorMat_->SetUniformBufferData(&ubo_);
    colorMesh_->SetMaterial(colorMat_);


    grayMesh_ = Mesh::Create();
    grayMesh_->SetIndices(plane.indices);
    grayMesh_->SetVertices(plane.vertices);
    grayMesh_->Init();
    grayMesh_->SetPosition(Vec3(-5, 0, 0));
    grayMesh_->SetRotation(Vec3(0, 0, 0));
    grayMesh_->SetScale(Vec3(10.0f));
    grayMat_ = Material::Create
    (
        "Standard.vert", "UnlitTexture.frag",
        { tex_ }
    );
    grayMesh_->SetMaterial(grayMat_);
}


bool Posterization::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    colorMesh_->Draw(camera_);
    grayMesh_->Draw(camera_);

    OrbitalControl::SetOrbit(3.14f, -4.71f, 23.5f);
    OrbitalControl::Update(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
        GUI::FloatSlider(ubo_.resolution, "resolution", 0.0f, 1.0f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Posterization::Release()
{
    tex_ = nullptr;
    colorMesh_ = nullptr;
    colorMat_ = nullptr;
    grayMesh_ = nullptr;
    grayMat_ = nullptr;
}

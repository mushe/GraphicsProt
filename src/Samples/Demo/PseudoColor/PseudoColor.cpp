#include "PseudoColor.h"


void PseudoColor::Start(shared_ptr<Engine> engine)
{
    tex_ = Texture::FromPNG("../Textures/Milwaukee_City_Hall_Old_Public_Domain_Photo.png");
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
        "Standard.vert", "PseudoColor.frag",
        sizeof(PseudoColorUBO),
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


bool PseudoColor::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    OrbitalControl::SetOrbit(3.14f, -4.71f, 23.5f);
    OrbitalControl::Update(camera_);

    engine->BeginRenderToScreen();
    colorMesh_->Draw(camera_);
    grayMesh_->Draw(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::FloatSlider(ubo_.colorLerp, "colorLerp", 0.0f, 1.0f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void PseudoColor::Release()
{
    colorMesh_ = nullptr;
    grayMesh_ = nullptr;
    tex_ = nullptr;
    colorMat_ = nullptr;
    grayMat_ = nullptr;
}

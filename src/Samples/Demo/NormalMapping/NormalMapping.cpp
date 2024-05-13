#include "NormalMapping.h"


void NormalMapping::Start(shared_ptr<Engine> engine)
{
    floorMesh_ = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    floorMesh_->SetIndices(plane.indices);
    floorMesh_->SetVertices(plane.vertices);
    floorMesh_->Init();
    floorMesh_->SetRotation(Vec3(0, 0, 0));
    floorMesh_->SetScale(Vec3(10.0f));
    floorTex_ = Texture::FromPNG("../Textures/NormalMap.png");
    floorMat_ = Material::Create
    (
        "Standard.vert", "NormalMap.frag",
        sizeof(NormalMapUBO),
        { floorTex_ }
    );
    floorMat_->SetUniformBufferData(&ubo_);
    floorMesh_->SetMaterial(floorMat_);

    OrbitalControl::SetOrbit(0.95f, 8.0f, 17.0f);
    OrbitalControl::Update(camera_);

}


bool NormalMapping::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();
    OrbitalControl::Update(camera_);

    bool closeScene = false;


    engine->BeginRenderToScreen();
    floorMesh_->Draw(camera_);

    ubo_.cameraPos = camera_.transform_.position;

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Vector(ubo_.lightDir, "lightDir", 0.01f);
        GUI::Float(ubo_.diffuseIntensity, "diffuseIntensity", 0.01f);
        GUI::Float(ubo_.diffusePower, "diffusePower", 0.01f);
        GUI::Float(ubo_.specularIntensity, "specularIntensity", 0.01f);
        GUI::Float(ubo_.specularPower, "specularPower", 0.01f);
        GUI::Color(ubo_.baseColor, "baseColor");

        GUI::Vector(meshRotation_, "meshRotation", 0.01f);
        floorMesh_->SetRotation(meshRotation_);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void NormalMapping::Release()
{
    floorMesh_ = nullptr;
    floorMat_ = nullptr;
    floorTex_ = nullptr;
}

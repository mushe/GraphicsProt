#include "Grass.h"


void Grass::Start(shared_ptr<Engine> engine)
{
    floorMesh_ = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane();
    floorMesh_->SetIndices(plane.indices);
    floorMesh_->SetVertices(plane.vertices);
    floorMesh_->Init();
    floorMesh_->SetRotation(Vec3(0, 0, 0));
    floorMesh_->SetScale(Vec3(10.0f));
    floorTex_ = Texture::FromPNG("../Textures/grid.png");
    floorMat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { floorTex_ });
    floorMesh_->SetMaterial(floorMat_);


    float posRange = 4.0f;
    for (int i = 0; i < 200; i++)
    {
        auto mesh = Mesh::FromOBJ("../Models/Grass/Grass.obj", 4);
        mesh->SetPosition(Vec3(Random::Range(-posRange, posRange), 0.0, Random::Range(-posRange, posRange)));
        mesh->SetRotation(Vec3(0, Random::Range(0.0f, 2.0f * 3.1415f), 0));
        mesh->SetScale(Vec3(1.0f));
        auto tex = Texture::FromPNG("../Models/Grass/Grass.png");
        auto mat = Material::Create("Grass.vert", "unlitTextureAlphaMasked.frag", sizeof(GrassUBO), { tex });

        auto ubo = std::make_shared<GrassUBO>();
        ubo->noiseTimeOffset = Random::Range(0.0f, 1000.0f);
        mat->SetUniformBufferData(ubo.get());

        mesh->SetMaterial(mat);

        meshes_.push_back(mesh);
        materials_.push_back(mat);
        ubos_.push_back(ubo);
    }

    OrbitalControl::SetOrbit(0.95f, 8.20f, 9.83f);
}


bool Grass::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();
    OrbitalControl::Update(camera_);

    bool closeScene = false;


    engine->BeginRenderToScreen();

    floorMesh_->Draw(camera_);

    for (auto m : meshes_)
        m->Draw(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Float(noiseSpeed_, "noiseSpeed", 0.01f);
        GUI::Float(noiseScale_, "noiseScale", 0.01f);

        for (auto& ubo : ubos_)
        {
            ubo->noiseSpeed = noiseSpeed_;
            ubo->noiseScale = noiseScale_;
        }
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Grass::Release()
{
    floorMesh_ = nullptr;
    floorTex_ = nullptr;
    floorMat_ = nullptr;
    for (auto& m : meshes_)
        m = nullptr;
    for (auto& m : materials_)
        m = nullptr;
    for (auto& u : ubos_)
        u = nullptr;

    meshes_.clear();
    materials_.clear();
    ubos_.clear();
}

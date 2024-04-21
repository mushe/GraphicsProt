#include "Core/Engine.h"

struct GrassUBO :public UniformBufferBase
{
    float noiseSpeed = 0.5f;
    float noiseScale = 1.8f;
    float noiseTimeOffset = 0.0f;
};

int main()
{
    auto engine = Engine::Init();

    auto floorMesh = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane();
    floorMesh->SetIndices(plane.indices);
    floorMesh->SetVertices(plane.vertices);
    floorMesh->Init();
    floorMesh->SetRotation(Vec3(0, 0, 0));
    floorMesh->SetScale(Vec3(10.0f));
    auto floorTex = Texture::FromPNG("../Textures/grid.png");
    auto floorMat = Material::Create("Standard.vert", "UnlitTexture.frag", { floorTex });
    floorMesh->SetMaterial(floorMat);


    std::vector<shared_ptr<Mesh>> meshes;
    std::vector<shared_ptr<Material>> materials;
    std::vector<shared_ptr<GrassUBO>> ubos;

    float posRange = 4.0f;
    for (int i = 0; i < 200; i++)
    {
        auto mesh = Mesh::FromOBJ("../Models/Grass/Grass.obj", 4);
        mesh->SetPosition(Vec3(Random::Range(-posRange, posRange), 0.0, Random::Range(-posRange, posRange)));
        mesh->SetRotation(Vec3(0, Random::Range(0.0f, 2.0f * 3.1415f),0));
        mesh->SetScale(Vec3(1.0f));
        auto tex = Texture::FromPNG("../Models/Grass/Grass.png");
        auto mat = Material::Create("Grass.vert", "unlitTextureAlphaMasked.frag", sizeof(GrassUBO), { tex });

        auto ubo = std::make_shared<GrassUBO>();
        ubo->noiseTimeOffset = Random::Range(0.0f, 1000.0f);
        mat->SetUniformBufferData(ubo.get());

        mesh->SetMaterial(mat);

        meshes.push_back(mesh);
        materials.push_back(mat);
        ubos.push_back(ubo);
    }

    float noiseScale = 0.96f;
    float noiseSpeed = 0.63f;

    Camera camera;
    OrbitalControl::SetOrbit(0.95f, 8.20f, 9.83f);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        engine->BeginRenderToScreen();
        floorMesh->Draw(camera);

        for(auto m : meshes) 
            m->Draw(camera);

        engine->OnGUI([&]()
        {
			GUI::Float(noiseSpeed, "noiseSpeed", 0.01f);
			GUI::Float(noiseScale, "noiseScale", 0.01f);

            for (auto& ubo : ubos)
            {
				ubo->noiseSpeed = noiseSpeed;
				ubo->noiseScale = noiseScale;
			}
        });

        engine->EndRenderToScreen();


        engine->EndFrame();
    }

    engine->Terminate();
}

#include "Core/Engine.h"

struct KiraUBO :public UniformBufferBase
{
    float division = 10.0f;
    float scrollSpeed = 0.2f;
};

int main()
{
    auto engine = Engine::Init();

    auto floorMesh = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    floorMesh->SetIndices(plane.indices);
    floorMesh->SetVertices(plane.vertices);
    floorMesh->Init();
    floorMesh->SetRotation(Vec3(0, 0, 0));
    floorMesh->SetScale(Vec3(10.0f));
    auto gradTex = Texture::FromPNG("../Textures/KiraGrad.png");
    auto floorMat = Material::Create
    (
        "Standard.vert", "KiraSticker.frag", 
        sizeof(KiraUBO),
        { gradTex }
    );
    KiraUBO ubo{};
    floorMat->SetUniformBufferData(&ubo);
    floorMesh->SetMaterial(floorMat);

    Camera camera;
    OrbitalControl::Update(camera);
    OrbitalControl::SetOrbit(0.0f, 0.0f, 12.0f);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera, 0.0f, 0.0f);

        engine->BeginRenderToScreen();
        floorMesh->Draw(camera);

        engine->OnGUI([&]()
        {
            GUI::Float(ubo.division, "division", 1.00f);
            GUI::Float(ubo.scrollSpeed, "scrollSpeed", 0.01f);
        });

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

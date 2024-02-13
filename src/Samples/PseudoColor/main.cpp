#include "Core/Engine.h"

struct PseudoColorUBO :public UniformBufferBase
{
    float colorLerp = 1.0f;
};

int main()
{
    auto engine = Engine::Init();

    auto plane = ModelGenerator::GeneratePlane(100);

    auto tex = Texture::FromPNG("../Textures/Milwaukee_City_Hall_Old_Public_Domain_Photo.png");

    auto colorMesh = Mesh::Create();
    colorMesh->SetIndices(plane.indices);
    colorMesh->SetVertices(plane.vertices);
    colorMesh->Init();
    colorMesh->SetPosition(Vec3(5, 0, 0));
    colorMesh->SetRotation(Vec3(0, 0, 0));
    colorMesh->SetScale(Vec3(10.0f));
    auto colorMat = Material::Create
    (
        "Standard.vert", "PseudoColor.frag", 
        sizeof(PseudoColorUBO),
        { tex }
    );
    PseudoColorUBO ubo{};
    colorMat->SetUniformBufferData(&ubo);
    colorMesh->SetMaterial(colorMat);


    auto grayMesh = Mesh::Create();
    grayMesh->SetIndices(plane.indices);
    grayMesh->SetVertices(plane.vertices);
    grayMesh->Init();
    grayMesh->SetPosition(Vec3(-5, 0, 0));
    grayMesh->SetRotation(Vec3(0, 0, 0));
    grayMesh->SetScale(Vec3(10.0f));
    auto grayMat = Material::Create
    (
        "Standard.vert", "UnlitTexture.frag", 
        { tex }
    );
    grayMesh->SetMaterial(grayMat);


    Camera camera;
    OrbitalControl::Update(camera);
    OrbitalControl::SetOrbit(3.14f, -4.71f, 23.5f);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        engine->BeginRenderToScreen();
        colorMesh->Draw(camera);
        grayMesh->Draw(camera);

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

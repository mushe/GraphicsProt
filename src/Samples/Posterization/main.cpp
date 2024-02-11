#include "Core/Engine.h"

struct PosterizationUBO :public UniformBufferBase
{
    float resolution = 0.081f;
};

int main()
{
    auto engine = Engine::Init();

    auto plane = ModelGenerator::GeneratePlane(100);

    auto tex = Texture::FromPNG("../Textures/mountain.png");

    auto colorMesh = Mesh::Create();
    colorMesh->SetIndices(plane.indices);
    colorMesh->SetVertices(plane.vertices);
    colorMesh->Init();
    colorMesh->SetPosition(Vec3(5, 0, 0));
    colorMesh->SetRotation(Vec3(0, 0, 0));
    colorMesh->SetScale(Vec3(10.0f));
    auto colorMat = Material::Create
    (
        "Standard.vert", "Posterization.frag", 
        sizeof(PosterizationUBO),
        { tex }
    );
    PosterizationUBO ubo{};
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
    OrbitalControl::SetOrbit(3.14f, -4.71f, 15.0f);
    OrbitalControl::Update(camera);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();
        colorMesh->Draw(camera);
        grayMesh->Draw(camera);

        engine->OnGUI([&]()
        {
            GUI::FloatSlider(ubo.resolution, "resolution", 0.0f, 1.0f);
		});

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

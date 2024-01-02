#include "Core/Engine.h"

struct NormalMapUBO :public UniformBufferBase
{
    glm::vec3 lightDir = glm::vec3(0.85f, 2.2f, 3.67f);
    float diffuseIntensity = 0.12f;

    glm::vec3 baseColor = glm::vec3(0.0f, 0.0f, 0.0f);
    float diffusePower = 1.0f;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0, 0.0);
    float specularIntensity = 0.48f;
  
    float specularPower = 5.6f;
};

int main()
{
    auto engine = Engine::Init();

    auto floorMesh = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    floorMesh->SetIndices(plane.indices);
    floorMesh->SetVertices(plane.vertices);
    floorMesh->Init();
    floorMesh->SetRotation(glm::vec3(0, 0, 0));
    floorMesh->SetScale(glm::vec3(10.0f));
    auto floorTex = Texture::FromPNG("../Textures/NormalMap.png");
    auto floorMat = Material::Create
    (
        "standard.vert", "normalMap.frag", 
        sizeof(NormalMapUBO),
        { floorTex }
    );
    NormalMapUBO ubo{};
    floorMat->SetUniformBufferData(&ubo);
    floorMesh->SetMaterial(floorMat);

    Camera camera;
    OrbitalControl::Update(camera);

    glm::vec3 meshRotation = glm::vec3(0, 0, 0);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        engine->BeginRenderToScreen();
        floorMesh->Draw(camera);
        engine->EndRenderToScreen();

        ubo.cameraPos = camera.transform_.position;

        engine->OnGUI([&]()
        {
            GUI::Vector(ubo.lightDir, "lightDir", 0.01f);
            GUI::Float(ubo.diffuseIntensity, "diffuseIntensity", 0.01f);
            GUI::Float(ubo.diffusePower, "diffusePower", 0.01f);
            GUI::Float(ubo.specularIntensity, "specularIntensity", 0.01f);
            GUI::Float(ubo.specularPower, "specularPower", 0.01f);
            GUI::Color(ubo.baseColor, "baseColor");

            GUI::Vector(meshRotation, "meshRotation", 0.01f);
            floorMesh->SetRotation(meshRotation);
        });

        engine->EndFrame();
    }

    engine->Terminate();
}

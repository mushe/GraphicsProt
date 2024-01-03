#include "Core/Engine.h"

struct CustomUBO :public UniformBufferBase
{
    float scale = 1.0f;
};

struct InstancingBuffer : InstancingUniformBufferBase
{
    glm::vec3 pos = glm::vec3(0);
    float scale = 1.0f;
};

int main()
{
    auto engine = Engine::Init();

    auto plane = ModelGenerator::GeneratePlane();
    auto mesh = Mesh::Create();
    mesh->SetIndices(plane.indices);
    mesh->SetVertices(plane.vertices);
    mesh->Init();
    const int instanceCount = 10000;
    auto mat = Material::Create(
        "Instancing.vert", "Unlit.frag", 
        sizeof(CustomUBO),
        sizeof(InstancingBuffer) * instanceCount,
        {}
    );

    // create instancing buffer
    std::vector<InstancingBuffer> instancingBuffer(instanceCount);

    float posRange = 5.0f;
    float scaleMin = 0.05f;
    float scaleMax = 0.2f;
    for (int i = 0; i < instanceCount; ++i)
    {
        instancingBuffer[i].pos = glm::vec3
        (
            Random::Range(-posRange, posRange),
            Random::Range(-posRange, posRange),
            Random::Range(-posRange, posRange)
        );
        instancingBuffer[i].scale = Random::Range(scaleMin, scaleMax);
    }
    mat->SetInstancingUniformBuffer(instancingBuffer.data());


    // create custom uniform buffer
    CustomUBO ubo;
    mat->SetUniformBufferData(&ubo);

    mesh->SetMaterial(mat);

    Camera camera;
    OrbitalControl::SetOrbit(0.12f, 48.2f, 7.6f);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        OrbitalControl::Update(camera);

        engine->BeginRenderToScreen();
        mesh->Draw(camera, instanceCount);
        engine->EndRenderToScreen();

        engine->OnGUI([&]()
        {
            GUI::Float(ubo.scale, "scale", 0.01f);
        });

        engine->EndFrame();
    }

    engine->Terminate();
}

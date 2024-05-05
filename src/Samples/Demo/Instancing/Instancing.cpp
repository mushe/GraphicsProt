#include "Instancing.h"


void Instancing::Start(shared_ptr<Engine> engine)
{
    instancingBuffer.resize(instanceCount_);

    auto plane = ModelGenerator::GeneratePlane();
    mesh_ = Mesh::Create();
    mesh_->SetIndices(plane.indices);
    mesh_->SetVertices(plane.vertices);
    mesh_->Init();
    
    mat_ = Material::Create(
        "Instancing.vert", "Unlit.frag",
        sizeof(CustomUBO),
        sizeof(InstancingBuffer) * instanceCount_,
        {}
    );

    float posRange = 5.0f;
    float scaleMin = 0.05f;
    float scaleMax = 0.2f;
    for (int i = 0; i < instanceCount_; ++i)
    {
        instancingBuffer[i].pos = Vec3
        (
            Random::Range(-posRange, posRange),
            Random::Range(-posRange, posRange),
            Random::Range(-posRange, posRange)
        );
        instancingBuffer[i].scale = Random::Range(scaleMin, scaleMax);
    }
    mat_->SetInstancingUniformBuffer(instancingBuffer.data());


    // create custom uniform buffer
    mat_->SetUniformBufferData(&ubo_);
    mesh_->SetMaterial(mat_);

    OrbitalControl::SetOrbit(0.12f, 48.2f, 7.6f);
    OrbitalControl::Update(camera_);

    GUI::ChangeStyleToLight();
}


bool Instancing::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();

    engine->BeginFrame();
    OrbitalControl::Update(camera_);

    bool closeScene = false;

    engine->BeginRenderToScreen();
    mesh_->Draw(camera_, instanceCount_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Float(ubo_.scale, "scale", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Instancing::Release()
{
}

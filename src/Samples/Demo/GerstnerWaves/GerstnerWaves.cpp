#include "GerstnerWaves.h"


void GerstnerWaves::Start(shared_ptr<Engine> engine)
{
    GUI::ChangeStyleToLight();

    // wave mesh
    ModelGeneratorData wavePlane = ModelGenerator::GeneratePlane(1000);
    waveMesh_ = Mesh::Create();
    waveMesh_->SetIndices(wavePlane.indices);
    waveMesh_->SetVertices(wavePlane.vertices);
    waveMesh_->Init();
    waveMesh_->SetPosition(Vec3(-0.2f, 0.45f, 0.0f));
    waveMesh_->SetRotation(Vec3(0.285f, 0.0f, 0.0f));
    waveMesh_->SetScale(Vec3(4.22f, 1.0f, 4.28f));
    waveMat_ = Material::Create
    (
        "GerstnerWaves.vert", "GerstnerWaves.frag",
        sizeof(GerstnerWaveUBO),
        {  }
    );
    waveMat_->SetUniformBufferData(&waveUBO_);
    waveMesh_->SetMaterial(waveMat_);

    // bg mesh
    auto bgPlane = ModelGenerator::GeneratePlane();
    bgMesh_ = Mesh::Create();
    bgMesh_->SetIndices(bgPlane.indices);
    bgMesh_->SetVertices(bgPlane.vertices);
    bgMesh_->Init();
    bgMesh_->SetPosition(Vec3(0.20f, 1.53f, -1.46f));
    bgMesh_->SetRotation(Vec3(-1.57f, 0.0f, 0.0f));
    bgMesh_->SetScale(Vec3(7.14f, 1.0f, 1.56f));
    auto tex = Texture::FromPNG("../Textures/ocean.png");
    bgMat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { tex });
    bgMesh_->SetMaterial(bgMat_);

    
    camera_.transform_.position = Vec3(0.0f, 0.0f, 1.91f);
    camera_.transform_.rotation = Vec3(0.0f, 0.0f, -1.0f);
}


bool GerstnerWaves::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();

    engine->BeginFrame();

    // rendering
    engine->BeginRenderToScreen();
    waveMesh_->Draw(camera_);
    bgMesh_->Draw(camera_);

    bool closeScene = false;

    engine->OnGUI([&]()
    {
        if(GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Label("Wave 1");
        GUI::FloatSlider(waveUBO_.waveHeight_1, "waveHeight_1", 0.0f, 0.05f);
        GUI::Float(waveUBO_.wavePosScale_1, "wavePosScale_1", 0.1f);
        GUI::Float(waveUBO_.waveSpeed_1, "waveSpeed_1", 0.01f);
        GUI::Vector(waveUBO_.waveDirection_1, "waveDirection_1", 0.01f);

        GUI::Label("Wave 2");
        GUI::FloatSlider(waveUBO_.waveHeight_2, "waveHeight_2", 0.0f, 0.05f);
        GUI::Float(waveUBO_.wavePosScale_2, "wavePosScale_2", 0.1f);
        GUI::Float(waveUBO_.waveSpeed_2, "waveSpeed_2", 0.01f);
        GUI::Vector(waveUBO_.waveDirection_2, "waveDirection_2", 0.01f);

        GUI::Label("Wave 3");
        GUI::FloatSlider(waveUBO_.waveHeight_3, "waveHeight_3", 0.0f, 0.05f);
        GUI::Float(waveUBO_.wavePosScale_3, "wavePosScale_3", 0.1f);
        GUI::Float(waveUBO_.waveSpeed_3, "waveSpeed_3", 0.01f);
        GUI::Vector(waveUBO_.waveDirection_3, "waveDirection_3", 0.01f);

        GUI::Label("Wave 4");
        GUI::FloatSlider(waveUBO_.waveHeight_4, "waveHeight_4", 0.0f, 0.05f);
        GUI::Float(waveUBO_.wavePosScale_4, "wavePosScale_4", 0.1f);
        GUI::Float(waveUBO_.waveSpeed_4, "waveSpeed_4", 0.01f);
        GUI::Vector(waveUBO_.waveDirection_4, "waveDirection_4", 0.01f);

        GUI::Label("Wave 5");
        GUI::FloatSlider(waveUBO_.waveHeight_5, "waveHeight_5", 0.0f, 0.05f);
        GUI::Float(waveUBO_.wavePosScale_5, "wavePosScale_5", 0.1f);
        GUI::Float(waveUBO_.waveSpeed_5, "waveSpeed_5", 0.01f);
        GUI::Vector(waveUBO_.waveDirection_5, "waveDirection_5", 0.01f);

        GUI::Label("--------------------------");
        GUI::Label("Wave look");
        GUI::Color(waveUBO_.waveColor, "waveColor");
        GUI::Color(waveUBO_.lightColor, "lightColor");
        GUI::Vector(waveUBO_.lightDirection, "lightDirection", 0.01f);
        GUI::Float(waveUBO_.diffusePower, "diffusePower", 0.01f);
        GUI::FloatSlider(waveUBO_.diffuseMultiply, "diffuseMultiply", 0.0f, 0.5f);
        GUI::Float(waveUBO_.specularPower, "specularPower", 0.01f);
        GUI::FloatSlider(waveUBO_.specularMultiply, "specularMultiply", 0.0f, 0.5f);
        GUI::Vector(waveUBO_.cameraPos, "cameraPos", 0.01f);
        GUI::Label("--------------------------");
        GUI::Label("Sun");
        GUI::Float(waveUBO_.sunIntensity, "sunIntensity", 0.01f);
        GUI::Color(waveUBO_.sunColor, "sunColor");
        GUI::Float(waveUBO_.sunSpread, "sunSpread", 0.01f);
        GUI::Float(waveUBO_.sunRefrectionThreshold, "sunRefrectionThreshold", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();

    return closeScene;
}


void GerstnerWaves::Release()
{
}

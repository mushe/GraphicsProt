#include "GerstnerWaves.h"


void GerstnerWaves::Start(shared_ptr<Engine> engine)
{
    Debug::Log("GerstnerWaves::Start()");

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
    waveMat = Material::Create
    (
        "GerstnerWaves.vert", "GerstnerWaves.frag",
        sizeof(GerstnerWaveUBO),
        {  }
    );
    waveMat->SetUniformBufferData(&waveUBO);
    waveMesh_->SetMaterial(waveMat);

    // bg mesh
    auto bgPlane = ModelGenerator::GeneratePlane();
    bgMesh = Mesh::Create();
    bgMesh->SetIndices(bgPlane.indices);
    bgMesh->SetVertices(bgPlane.vertices);
    bgMesh->Init();
    bgMesh->SetPosition(Vec3(0.20f, 1.53f, -1.46f));
    bgMesh->SetRotation(Vec3(-1.57f, 0.0f, 0.0f));
    bgMesh->SetScale(Vec3(7.14f, 1.0f, 1.56f));
    auto tex = Texture::FromPNG("../Textures/ocean.png");
    bgMat = Material::Create("Standard.vert", "UnlitTexture.frag", { tex });
    bgMesh->SetMaterial(bgMat);

    
    camera.transform_.position = Vec3(0.0f, 0.0f, 1.91f);
    camera.transform_.rotation = Vec3(0.0f, 0.0f, -1.0f);

}


bool GerstnerWaves::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

    engine->BeginFrame();

    // rendering
    engine->BeginRenderToScreen();
    waveMesh_->Draw(camera);
    bgMesh->Draw(camera);

    bool closeScene = false;

    engine->OnGUI([&]()
    {
        if(GUI::Button("Back To Menu")) closeScene = true;

        GUI::Label("Wave 1");
        GUI::FloatSlider(waveUBO.waveHeight_1, "waveHeight_1", 0.0f, 0.05f);
        GUI::Float(waveUBO.wavePosScale_1, "wavePosScale_1", 0.1f);
        GUI::Float(waveUBO.waveSpeed_1, "waveSpeed_1", 0.01f);
        GUI::Vector(waveUBO.waveDirection_1, "waveDirection_1", 0.01f);

        GUI::Label("Wave 2");
        GUI::FloatSlider(waveUBO.waveHeight_2, "waveHeight_2", 0.0f, 0.05f);
        GUI::Float(waveUBO.wavePosScale_2, "wavePosScale_2", 0.1f);
        GUI::Float(waveUBO.waveSpeed_2, "waveSpeed_2", 0.01f);
        GUI::Vector(waveUBO.waveDirection_2, "waveDirection_2", 0.01f);

        GUI::Label("Wave 3");
        GUI::FloatSlider(waveUBO.waveHeight_3, "waveHeight_3", 0.0f, 0.05f);
        GUI::Float(waveUBO.wavePosScale_3, "wavePosScale_3", 0.1f);
        GUI::Float(waveUBO.waveSpeed_3, "waveSpeed_3", 0.01f);
        GUI::Vector(waveUBO.waveDirection_3, "waveDirection_3", 0.01f);

        GUI::Label("Wave 4");
        GUI::FloatSlider(waveUBO.waveHeight_4, "waveHeight_4", 0.0f, 0.05f);
        GUI::Float(waveUBO.wavePosScale_4, "wavePosScale_4", 0.1f);
        GUI::Float(waveUBO.waveSpeed_4, "waveSpeed_4", 0.01f);
        GUI::Vector(waveUBO.waveDirection_4, "waveDirection_4", 0.01f);

        GUI::Label("Wave 5");
        GUI::FloatSlider(waveUBO.waveHeight_5, "waveHeight_5", 0.0f, 0.05f);
        GUI::Float(waveUBO.wavePosScale_5, "wavePosScale_5", 0.1f);
        GUI::Float(waveUBO.waveSpeed_5, "waveSpeed_5", 0.01f);
        GUI::Vector(waveUBO.waveDirection_5, "waveDirection_5", 0.01f);

        GUI::Label("--------------------------");
        GUI::Label("Wave look");
        GUI::Color(waveUBO.waveColor, "waveColor");
        GUI::Color(waveUBO.lightColor, "lightColor");
        GUI::Vector(waveUBO.lightDirection, "lightDirection", 0.01f);
        GUI::Float(waveUBO.diffusePower, "diffusePower", 0.01f);
        GUI::FloatSlider(waveUBO.diffuseMultiply, "diffuseMultiply", 0.0f, 0.5f);
        GUI::Float(waveUBO.specularPower, "specularPower", 0.01f);
        GUI::FloatSlider(waveUBO.specularMultiply, "specularMultiply", 0.0f, 0.5f);
        GUI::Vector(waveUBO.cameraPos, "cameraPos", 0.01f);
        GUI::Label("--------------------------");
        GUI::Label("Sun");
        GUI::Float(waveUBO.sunIntensity, "sunIntensity", 0.01f);
        GUI::Color(waveUBO.sunColor, "sunColor");
        GUI::Float(waveUBO.sunSpread, "sunSpread", 0.01f);
        GUI::Float(waveUBO.sunRefrectionThreshold, "sunRefrectionThreshold", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();

    return closeScene;
}


void GerstnerWaves::Release()
{
    Debug::Log("GerstnerWaves::Release()");
}

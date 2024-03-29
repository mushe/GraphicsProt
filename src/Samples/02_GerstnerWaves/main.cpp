#include "Core/Engine.h"

struct GerstnerWaveUBO :public UniformBufferBase
{
    Vec3 waveColor = Vec3(0.016f, 0.027f, 0.067f);
    float diffusePower = 1.0f;

    Vec3 lightDirection = Vec3(3.93f, 0.96f, -0.37f);
    float diffuseMultiply = 0.068f;

    Vec3 lightColor = Vec3(1.0f, 0.74f, 0.22f);
    float specularPower = 30.29f;

    Vec3 cameraPos = Vec3(1);
    float specularMultiply = 0.118f;

    float sunSpread = 0.14f;
    float sunRefrectionThreshold = -0.01f;
    float dummy1 = 1.0f;
    float dummy2 = 1.0f;

    Vec3 sunColor = Vec3(0.60f, 0.32f, 0.0f);
    float sunIntensity = 16.0f;

    float waveHeight_1 = 0.001f;
    float waveHeight_2 = 0.002f;
    float waveHeight_3 = 0.002f;
    float waveHeight_4 = 0.004f;

    float waveHeight_5 = 0.013f;
    float wavePosScale_1 = 226.3f;
    float wavePosScale_2 = 208.6f;
    float wavePosScale_3 = 127.8f;

    float wavePosScale_4 = 122.1f;
    float wavePosScale_5 = 12.6f;
    float waveSpeed_1 = 1.97f;
    float waveSpeed_2 = 0.35f;

    float waveSpeed_3 = 0.77f;
    float waveSpeed_4 = 7.78f;
    float waveSpeed_5 = 3.24f;
    float dummy = 1.0f;

    Vec2 waveDirection_1 = Vec2(1.0f, 0.24f);
    Vec2 waveDirection_2 = Vec2(0.82f, 0.97f);
    Vec2 waveDirection_3 = Vec2(1.0f, -1.62f);
    Vec2 waveDirection_4 = Vec2(1.23f, 3.52f);
    Vec2 waveDirection_5 = Vec2(3.13f, 2.80f);
};

int main()
{
    auto engine = Engine::Init();
    GUI::ChangeStyleToLight();

    // wave mesh
    auto wavePlane = ModelGenerator::GeneratePlane(1000);
    auto waveMesh = Mesh::Create();
    waveMesh->SetIndices(wavePlane.indices);
    waveMesh->SetVertices(wavePlane.vertices);
    waveMesh->Init();
    waveMesh->SetPosition(Vec3(-0.2f, 0.45f, 0.0f));
    waveMesh->SetRotation(Vec3(0.285f, 0.0f, 0.0f));
    waveMesh->SetScale(Vec3(4.22f, 1.0f, 4.28f));
    auto waveMat = Material::Create
    (
        "GerstnerWaves.vert", "GerstnerWaves.frag",
        sizeof(GerstnerWaveUBO),
        {  }
    );
    GerstnerWaveUBO waveUBO{};
    waveMat->SetUniformBufferData(&waveUBO);
    waveMesh->SetMaterial(waveMat);

    // bg mesh
    auto bgPlane = ModelGenerator::GeneratePlane();
    auto bgMesh = Mesh::Create();
    bgMesh->SetIndices(bgPlane.indices);
    bgMesh->SetVertices(bgPlane.vertices);
    bgMesh->Init();
    bgMesh->SetPosition(Vec3(0.20f, 1.53f, -1.46f));
    bgMesh->SetRotation(Vec3(-1.57f, 0.0f, 0.0f));
    bgMesh->SetScale(Vec3(7.14f, 1.0f, 1.56f));
    auto tex = Texture::FromPNG("../Textures/ocean.png");
    auto bgMat = Material::Create("Standard.vert", "UnlitTexture.frag", { tex });
    bgMesh->SetMaterial(bgMat);

    Camera camera;
    camera.transform_.position = Vec3(0.0f, 0.0f, 1.91f);
    camera.transform_.rotation = Vec3(0.0f, 0.0f, -1.0f);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        //OrbitalControl::Update(camera);

        // rendering
        engine->BeginRenderToScreen();
        waveMesh->Draw(camera);
        bgMesh->Draw(camera);

        engine->OnGUI([&]()
        {
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
    }

    engine->Terminate();
}

#include "Core/Engine.h"

struct RiverUBO :public UniformBufferBase
{
    float distortionSpeed = 0.0f;
    float distortionScale = 0.0f;
    float distortionPosScale = 0.0f;
    float noiseScaleX_1 = 0.0f;

    float noiseScaleY_1 = 0.0f;
    float noiseSpeed_1 = 0.0f;
    float noiseScaleX_2 = 0.0f;
    float noiseScaleY_2 = 0.0f;

    float noiseSpeed_2 = 0.0f;
    float noiseThreshold = 0.0f;
    float noisePower = 0.0f;
    float noiseMultiply = 0.0f;
    
    float colorLerp = 0.0f;
    float riverColR = 0.0f;
    float riverColG = 0.0f;
    float riverColB = 0.0f;
};


float colorLerp = 0.370000;
float distortionPosScale = 90.000000;
float distortionScale = 0.005000;
float distortionSpeed = 5.570000;
float noiseMultiply = 14.560000;
float noisePower = 4.120000;
float noiseScaleX_1 = 6.030000;
float noiseScaleX_2 = 3.430000;
float noiseScaleY_1 = 18.530001;
float noiseScaleY_2 = 10.640000;
float noiseSpeed_1 = 2.430000;
float noiseSpeed_2 = 2.800000;
float noiseThreshold = 0.270000;
float riverColB = 0.413000;
float riverColG = 0.260000;
float riverColR = 0.194000;

int main()
{
    auto engine = Engine::Init();

    auto mesh = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    mesh->SetIndices(plane.indices);
    mesh->SetVertices(plane.vertices);
    mesh->Init();
    mesh->SetRotation(Vec3(0, 0, 0));
    mesh->SetScale(Vec3(10.0f));
    auto tex1 = Texture::FromPNG("../Textures/RiverGround.png");
    auto tex2 = Texture::FromPNG("../Textures/RiverMask.png");
    auto mat = Material::Create
    (
        "Standard.vert", "river.frag", 
        sizeof(RiverUBO),
        { tex1, tex2 }
    );
    RiverUBO ubo{};
    mat->SetUniformBufferData(&ubo);
    mesh->SetMaterial(mat);

    Camera camera;
    OrbitalControl::Update(camera);

    Vec3 meshRotation = Vec3(0, 0, 0);

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        //OrbitalControl::Update(camera, 0.0f, 0.0f);

        engine->BeginRenderToScreen();
        mesh->Draw(camera);


        engine->OnGUI([&]()
        {
            // GUI::Float(ubo.distortionSpeed, "distortionSpeed", 0.01f);
            // GUI::Float(ubo.distortionScale, "distortionScale", 0.01f);
            // GUI::Float(ubo.distortionPosScale, "distortionPosScale", 0.01f);
            // GUI::Float(ubo.noiseScaleX_1, "noiseScaleX_1", 0.01f);
            // GUI::Float(ubo.noiseScaleY_1, "noiseScaleY_1", 0.01f);
            // GUI::Float(ubo.noiseSpeed_1, "noiseSpeed_1", 0.01f);
            // GUI::Float(ubo.noiseScaleX_2, "noiseScaleX_2", 0.01f);
            // GUI::Float(ubo.noiseScaleY_2, "noiseScaleY_2", 0.01f);
            // GUI::Float(ubo.noiseSpeed_2, "noiseSpeed_2", 0.01f);
            // GUI::Float(ubo.noiseThreshold, "noiseThreshold", 0.01f);
            // GUI::Float(ubo.noisePower, "noisePower", 0.01f);
            // GUI::Float(ubo.noiseMultiply, "noiseMultiply", 0.01f);
            // GUI::FloatSlider(ubo.colorLerp, "colorLerp", 0.0f, 1.0f);
            // GUI::FloatSlider(ubo.riverColR, "riverColR", 0.0f, 1.0f);
            // GUI::FloatSlider(ubo.riverColG, "riverColG", 0.0f, 1.0f);
            // GUI::FloatSlider(ubo.riverColB, "riverColB", 0.0f, 1.0f);

            GUI::Float(distortionSpeed, "distortionSpeed", 0.01f);
            GUI::Float(distortionScale, "distortionScale", 0.01f);
            GUI::Float(distortionPosScale, "distortionPosScale", 0.01f);
            GUI::Float(noiseScaleX_1, "noiseScaleX_1", 0.01f);
            GUI::Float(noiseScaleY_1, "noiseScaleY_1", 0.01f);
            GUI::Float(noiseSpeed_1, "noiseSpeed_1", 0.01f);
            GUI::Float(noiseScaleX_2, "noiseScaleX_2", 0.01f);
            GUI::Float(noiseScaleY_2, "noiseScaleY_2", 0.01f);
            GUI::Float(noiseSpeed_2, "noiseSpeed_2", 0.01f);
            GUI::Float(noiseThreshold, "noiseThreshold", 0.01f);
            GUI::Float(noisePower, "noisePower", 0.01f);
            GUI::Float(noiseMultiply, "noiseMultiply", 0.01f);
            GUI::FloatSlider(colorLerp, "colorLerp", 0.0f, 1.0f);
            GUI::FloatSlider(riverColR, "riverColR", 0.0f, 1.0f);
            GUI::FloatSlider(riverColG, "riverColG", 0.0f, 1.0f);
            GUI::FloatSlider(riverColB, "riverColB", 0.0f, 1.0f);
            ubo.distortionSpeed = distortionSpeed;
            ubo.distortionScale = distortionScale;
            ubo.distortionPosScale = distortionPosScale;
            ubo.noiseScaleX_1 = noiseScaleX_1;
            ubo.noiseScaleY_1 = noiseScaleY_1;
            ubo.noiseSpeed_1 = noiseSpeed_1;
            ubo.noiseScaleX_2 = noiseScaleX_2;
            ubo.noiseScaleY_2 = noiseScaleY_2;
            ubo.noiseSpeed_2 = noiseSpeed_2;
            ubo.noiseThreshold = noiseThreshold;
            ubo.noisePower = noisePower;
            ubo.noiseMultiply = noiseMultiply;
            ubo.colorLerp = colorLerp;
            ubo.riverColR = riverColR;
            ubo.riverColG = riverColG;
            ubo.riverColB = riverColB;

        });

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

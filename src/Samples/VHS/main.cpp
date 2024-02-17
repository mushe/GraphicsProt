#include "Core/Engine.h"

struct VhsUBO :public UniformBufferBase
{
    float scale = 3.3f;
    float leapThreshold = 0.40f;
    float leapScale = 12.0f;
    float leapSpeed = 3.5f;
};

struct ChromaticAberrationUBO :public UniformBufferBase
{
    float distance = 0.011f;
};

int main()
{
    auto engine = Engine::Init();

    // mesh
    auto mesh = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh->SetPosition(Vec3(0, -1.0f, 0));
    mesh->SetRotation(Vec3(0));
    mesh->SetScale(Vec3(0.1f));
    auto tex = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");
    auto mat = Material::Create("Standard.vert", "UnlitTexture.frag", { tex });
    mesh->SetMaterial(mat);


    // post process 1
    auto rt1 = RenderTarget::Create(1920, 1080);
    auto chromaticAberrationMat = Material::Create
    (
        "renderToScreen.vert", "PP_ChromaticAberration.frag",
        sizeof(ChromaticAberrationUBO),
        { rt1->GetColorTexture() }
    );
    ChromaticAberrationUBO chromaUbo{};
    chromaticAberrationMat->SetUniformBufferData(&chromaUbo);

    // post process 2
    auto rt2 = RenderTarget::Create(1920, 1080);
    auto vhsMat = Material::Create
    (
        "renderToScreen.vert", "PP_VHS.frag",
        sizeof(VhsUBO),
        { rt2->GetColorTexture() }
    );
    VhsUBO vhsUbo{};
    vhsMat->SetUniformBufferData(&vhsUbo);

    Camera camera;

    while (engine->Running())
    {
        engine->BeginFrame();

        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        // object rendering to render target
        rt1->BeginRenderToTexture();
        mesh->Draw(camera);
        rt1->EndRenderToTexture();


        engine->BlitToRenderTarget(chromaticAberrationMat, rt2);


        // post process rendering to screen
        engine->BeginRenderToScreen();
        engine->BlitToScreen(vhsMat);

        engine->OnGUI([&]()
        {
            GUI::FloatSlider(chromaUbo.distance, "chroma distance", 0.0f, 0.025f);
            GUI::FloatSlider(vhsUbo.scale, "scale", 0.0f, 15.0f);
            GUI::Float(vhsUbo.leapThreshold, "leapThreshold", 0.01f);
            GUI::Float(vhsUbo.leapScale, "leapScale", 0.01f);
            GUI::Float(vhsUbo.leapSpeed, "leapSpeed", 0.01f);
		});
        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

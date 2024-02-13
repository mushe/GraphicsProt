#include "Core/Engine.h"

struct PostProcessUBO :public UniformBufferBase
{
    float division = 16.0f;
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

    // post process
    auto rt = RenderTarget::Create(1920, 1080);
    auto postProcessMat = Material::Create
    (
        "renderToScreen.vert", "PP_Kaleidoscope.frag",
        sizeof(PostProcessUBO),
        { rt->GetColorTexture() }
    );
    PostProcessUBO ubo{};
    postProcessMat->SetUniformBufferData(&ubo);

    Camera camera;
    float t = 0.0f;
    OrbitalControl::SetOrbit(0.413713, 6.743663, 5.470689);

    while (engine->Running())
    {
        t += 0.016f;

        engine->BeginFrame();

        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);
        OrbitalControl::LogOrbit();
        OrbitalControl::SetRadius(5.470689 + sin(t) * 5.0f);

        // object rendering to render target
        rt->BeginRenderToTexture();
        mesh->Draw(camera);
        rt->EndRenderToTexture();

        // post process rendering to screen
        engine->BeginRenderToScreen();
        engine->BlitToScreen(postProcessMat);

        engine->OnGUI([&]()
        {
            GUI::Float(ubo.division, "division", 1.0f);
		});
        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

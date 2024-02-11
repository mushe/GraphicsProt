#include "Core/Engine.h"

struct PostProcessUBO :public UniformBufferBase
{
    float distance = 0.004f;
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
        "renderToScreen.vert", "PP_ChromaticAberration.frag",
        sizeof(PostProcessUBO),
        { rt->GetColorTexture() }
    );
    PostProcessUBO ubo{};
    postProcessMat->SetUniformBufferData(&ubo);

    Camera camera;

    while (engine->Running())
    {
        engine->BeginFrame();

        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        // object rendering to render target
        rt->BeginRenderToTexture();
        mesh->Draw(camera);
        rt->EndRenderToTexture();

        // post process rendering to screen
        engine->BeginRenderToScreen();
        engine->BlitToScreen(postProcessMat);

        engine->OnGUI([&]()
        {
            GUI::FloatSlider(ubo.distance, "distance", 0.0f, 0.02f);
		});
        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

#include "Core/Engine.h"

struct PostProcessUBO :public UniformBufferBase
{
    float postProcessLerp = 1.0f;
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
        "renderToScreen.vert", "PP_GrayScale.frag",
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

        engine->BlitToScreen(postProcessMat);

        engine->OnGUI([&]()
        {
            GUI::FloatSlider(ubo.postProcessLerp, "postProcessLerp", 0.0f, 1.0f);
		});

        engine->EndFrame();
    }

    engine->Terminate();
}

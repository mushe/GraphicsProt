#include "Core/Engine.h"

struct PostProcessUBO :public UniformBufferBase
{
    float scale = 5.42f;
    float speed = 1.4f;
};

int main()
{
    auto engine = Engine::Init();

    auto rt = RenderTarget::Create(1920, 1080);
    auto postProcessMat = Material::Create
    (
        "renderToScreen.vert", "PP_PerlinNoise.frag",
        sizeof(PostProcessUBO),
        {  }
    );
    PostProcessUBO ubo{};
    postProcessMat->SetUniformBufferData(&ubo);

    GUI::ChangeStyleToLight();


    while (engine->Running())
    {
        engine->BeginFrame();

        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();
        engine->BlitToScreen(postProcessMat);

        engine->OnGUI([&]()
        {
            GUI::Float(ubo.scale, "scale", 0.01f);
            GUI::Float(ubo.speed, "speed", 0.01f);
		});

        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

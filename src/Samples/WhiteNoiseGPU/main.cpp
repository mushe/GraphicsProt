#include "Core/Engine.h"

struct PostProcessUBO :public UniformBufferBase
{
    float postProcessLerp = 1.0f;
};

int main()
{
    auto engine = Engine::Init();

    auto rt = RenderTarget::Create(1920, 1080);
    auto postProcessMat = Material::Create
    (
        "renderToScreen.vert", "PP_WhiteNoise.frag",
        {  }
    );

    Camera camera;

    while (engine->Running())
    {
        engine->BeginFrame();

        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        OrbitalControl::Update(camera);

        engine->BeginRenderToScreen();
        engine->BlitToScreen(postProcessMat);
        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

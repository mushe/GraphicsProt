#include "PerlinNoise.h"


void PerlinNoise::Start(shared_ptr<Engine> engine)
{
    rt_ = RenderTarget::Create(1920, 1080);
    postProcessMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_PerlinNoise.frag",
        sizeof(PerlinNoiseUBO),
        {  }
    );
    postProcessMat_->SetUniformBufferData(&ubo_);

    GUI::ChangeStyleToLight();
}


bool PerlinNoise::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();
    engine->BlitToScreen(postProcessMat_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Float(ubo_.scale, "scale", 0.01f);
        GUI::Float(ubo_.speed, "speed", 0.01f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void PerlinNoise::Release()
{
}

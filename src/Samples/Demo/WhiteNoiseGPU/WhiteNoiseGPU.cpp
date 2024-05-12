#include "WhiteNoiseGPU.h"


void WhiteNoiseGPU::Start(shared_ptr<Engine> engine)
{
    rt_ = RenderTarget::Create(1920, 1080);
    postProcessMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_WhiteNoise.frag",
        {  }
    );

    GUI::ChangeStyleToLight();
}


bool WhiteNoiseGPU::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    OrbitalControl::Update(camera_);
    engine->BeginFrame();

    bool closeScene = false;

    engine->BeginRenderToScreen();
    engine->BlitToScreen(postProcessMat_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void WhiteNoiseGPU::Release()
{
    postProcessMat_ = nullptr;
    rt_ = nullptr;
}

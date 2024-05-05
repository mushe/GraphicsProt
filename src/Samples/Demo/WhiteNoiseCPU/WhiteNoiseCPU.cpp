#include "WhiteNoiseCPU.h"


void WhiteNoiseCPU::Start(shared_ptr<Engine> engine)
{
    positions_.resize(N_);

    for (int i = 0; i < N_; i++)
    {
        int x = i % GridNum_;
        int y = i / GridNum_;
        positions_[i] = Vec2(x * GridScale_, y * GridScale_) + Vec2(GridScale_ / 2.0f);
    }
}


bool WhiteNoiseCPU::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    for (int i = 0; i < positions_.size(); i++)
    {
        float r = Random::Range(0.0f, 1.0f);
        Vec4 color = Vec4(r, r, r, 0.2);
        ShapeDrawer::Rect(positions_[i], Vec2(GridScale_), color);
    }

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void WhiteNoiseCPU::Release()
{
}

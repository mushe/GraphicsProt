#include "TextRender.h"

void TextRender::Reset()
{
    t_ = 0.0f;
    positions_.clear();
    scales_.clear();
    colors_.clear();

    for (int i = 0; i < 100; i++)
    {
        // random position
        positions_.push_back(Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f)));
        scales_.push_back(Random::Range(1.0f, 5.0f));
        colors_.push_back(Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f));
    }
}


void TextRender::Start(shared_ptr<Engine> engine)
{
    Debug::Log("TextRender::Start()");
    Reset();
}


bool TextRender::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    t_ += 0.016f;
    if(t_ > 1.0f) Reset();


    engine->BeginRenderToScreen();

    for (int i = 0; i < positions_.size(); i++)
    {
        Text("ABC", positions_[i], scales_[i], colors_[i]);
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


void TextRender::Release()
{
    Debug::Log("TextRender::Release()");
    positions_.clear();
    scales_.clear();
    colors_.clear();
}

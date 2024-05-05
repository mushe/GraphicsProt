#include "Shape.h"


void Shape::Start(shared_ptr<Engine> engine)
{
    randIndices_.resize(N_);
    positions_.resize(N_);
    randScales_.resize(N_);
    randColors_.resize(N_);
    randDirections_.resize(N_);

    for (int i = 0; i < N_; i++)
    {
        int x = i % GridNum_;
        int y = i / GridNum_;

        randIndices_[i] = Random::Range(0, 3);
        positions_[i] = Vec2(x * GridScale_, y * GridScale_) + Vec2(GridScale_ / 2.0f);
        randScales_[i] = GridScale_;
        randColors_[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
        randDirections_[i] = Vec2(Random::Range(-1.0f, 1.0f), Random::Range(-1.0f, 1.0f));
    }
}


bool Shape::Update(shared_ptr<Engine> engine)
{
    t_ += 0.08f;

    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();
    bool closeScene = false;


    engine->BeginRenderToScreen();

    for (int i = 0; i < randIndices_.size(); i++)
    {
        float scaleMultiply = sin(t_ + i) * 0.5f + 0.5f;

        switch (randIndices_[i])
        {
        case 0:
            ShapeDrawer::Rect(positions_[i], Vec2(randScales_[i]) * scaleMultiply, randColors_[i]);
            break;
        case 1:
            ShapeDrawer::Circle(positions_[i], Vec2(randScales_[i]) * scaleMultiply, randColors_[i]);
            break;
        case 2:
            ShapeDrawer::Line(positions_[i] - Vec2(GridScale_ / 2.0f) * scaleMultiply, positions_[i] + Vec2(GridScale_ / 2.0f) * scaleMultiply, 1.0f, randColors_[i]);
            break;
        case 3:
            ShapeDrawer::Triangle(positions_[i], randDirections_[i], Vec2(randScales_[i]) * scaleMultiply, randColors_[i]);
            break;
        }
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


void Shape::Release()
{
}

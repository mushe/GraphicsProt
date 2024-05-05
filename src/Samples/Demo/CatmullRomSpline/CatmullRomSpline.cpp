#include "CatmullRomSpline.h"



Vec2 CatmullRomSpline::CatmullRomSplineInterp(Vec2 p_mi1, Vec2 p_0, Vec2 p_1, Vec2 p_2, float t)
{
    Vec2 a4 = p_0;
    Vec2 a3 = (p_1 - p_mi1) / 2.0f;
    Vec2 a1 = (p_2 - p_0) / 2.0f - 2.0f * p_1 + a3 + 2.0f * a4;
    Vec2 a2 = 3.0f * p_1 - (p_2 - p_0) / 2.0f - 2.0f * a3 - 3.0f * a4;

    return a1 * t * t * t + a2 * t * t + a3 * t + a4;
}

void CatmullRomSpline::Start(shared_ptr<Engine> engine)
{
}


bool CatmullRomSpline::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    // moving point
    t_ += 0.016f;
    auto p_0 = Vec2(0.3f + 0.24f * cos(t_), 0.25f + 0.22f * sin(t_ * 0.5));
    auto p_1 = Vec2(0.7f + 0.24f * cos(t_ + 3.32), 0.75f + 0.15f * sin(t_ * 0.2 + 1.02f));
    auto p_mi1 = Vec2(0.05f, 0.5f + 0.45f * sin(t_ * 2.3 + 1.7f));
    auto p_2 = Vec2(0.95f, 0.5f + 0.45f * sin(t_ * 3.3));
    float circleT = fmod(t_ * 0.35f, 1.0f);

    engine->BeginRenderToScreen();

    float circleScale = 0.025f;
    ShapeDrawer::Circle(p_mi1, Vec2(circleScale));
    ShapeDrawer::Circle(p_0, Vec2(circleScale));
    ShapeDrawer::Circle(p_1, Vec2(circleScale));
    ShapeDrawer::Circle(p_2, Vec2(circleScale));

    // spline interpolation
    int division = 30;
    for (int i = 0; i < division; i++)
    {
        float t0 = (float)i / (float)division;
        float t1 = (float)(i + 1) / (float)division;
        Vec2 p0 = CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, t0);
        Vec2 p1 = CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, t1);
        ShapeDrawer::Line(p0, p1, 1.0f);
    }

    // point on spline
    ShapeDrawer::Circle(CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, circleT), Vec2(circleScale * 1.5f), Vec4(0, 0.5, 1, 1));

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void CatmullRomSpline::Release()
{
}

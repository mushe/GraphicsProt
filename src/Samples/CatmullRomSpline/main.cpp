#include "Core/Engine.h"

Vec2 CatmullRomSplineInterp(Vec2 p_mi1, Vec2 p_0, Vec2 p_1, Vec2 p_2, float t)
{
    Vec2 a4 = p_0;
    Vec2 a3 = (p_1 - p_mi1) / 2.0f;
    Vec2 a1 = (p_2 - p_0) / 2.0f - 2.0f * p_1 + a3 + 2.0f * a4;
    Vec2 a2 = 3.0f * p_1 - (p_2 - p_0) / 2.0f - 2.0f * a3 - 3.0f * a4;

    return a1 * t * t * t + a2 * t * t + a3 * t + a4;
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    Vec2 p_mi1 = Vec2(0.05f, 0.5f);
    Vec2 p_0 = Vec2(0.3f, 0.25f);
    Vec2 p_1 = Vec2(0.7f, 0.75f);
    Vec2 p_2 = Vec2(0.95f, 0.5f);

    float t = 0.0f;

    while (engine->Running())
    {
        t += 0.016f;

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();

        float circleScale = 0.025f;
        ShapeDrawer::Circle(p_mi1, Vec2(circleScale));
        ShapeDrawer::Circle(p_0, Vec2(circleScale));
        ShapeDrawer::Circle(p_1, Vec2(circleScale));
        ShapeDrawer::Circle(p_2, Vec2(circleScale));

        // moving point
        p_0 = Vec2(0.3f + 0.24f * cos(t), 0.25f + 0.22f * sin(t * 0.5));
        p_1 = Vec2(0.7f + 0.24f * cos(t + 3.32), 0.75f + 0.15f * sin(t * 0.2 + 1.02f));
        p_mi1 = Vec2(0.05f, 0.5f + 0.45f * sin(t * 2.3 + 1.7f));
        p_2 = Vec2(0.95f, 0.5f + 0.45f * sin(t * 3.3));
        float circleT = fmod(t * 0.35f, 1.0f);

        // spline interpolation
        int division = 30;
        for(int i = 0; i < division; i++)
        {
            float t0 = (float)i / (float)division;
            float t1 = (float)(i + 1) / (float)division;
            Vec2 p0 = CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, t0);
            Vec2 p1 = CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, t1);
            ShapeDrawer::Line(p0, p1, 1.0f);
        }

        // point on spline
        ShapeDrawer::Circle(CatmullRomSplineInterp(p_mi1, p_0, p_1, p_2, circleT), Vec2(circleScale * 1.5f), Vec4(0,0.5,1,1));


        engine->OnGUI([&]()
        {
		});
        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

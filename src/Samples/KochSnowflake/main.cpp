#include "Core/Engine.h"

void KochCurve(Vec2 start, Vec2 end, int depth)
{
    if (depth == 0)
    {
        ShapeDrawer::Line(start, end);
        return;
    }

    Vec2 v1 = start + (end - start) / 3.0f;
    Vec2 v2 = start + (end - start) * 2.0f / 3.0f;
    Vec2 v3 = v1 + (v2 - v1) * 0.5f + Vec2((v2 - v1).y, -(v2 - v1).x) * sqrt(3.0f) / 2.0f;

    KochCurve(start, v1, depth - 1);
    KochCurve(v1, v3, depth - 1);
    KochCurve(v3, v2, depth - 1);
    KochCurve(v2, end, depth - 1);

}

void KochSnowflake(Vec2 p1, Vec2 p2, Vec2 p3, int depth)
{
    KochCurve(p2, p1, depth);
    KochCurve(p3, p2, depth);
    KochCurve(p1, p3, depth);
}


int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    float t = 1.0f;

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();


        float radius = 0.5f;
        Vec2 center = Vec2(0.5, 0.5);
        float d1 = -90.0f;
        float d2 = -210.0f;
        float d3 = -330.0f;
        auto p1 = Vec2(cos(glm::radians(d1)) * radius, sin(glm::radians(d1)) * radius) + center;
        auto p2 = Vec2(cos(glm::radians(d2)) * radius, sin(glm::radians(d2)) * radius) + center;
        auto p3 = Vec2(cos(glm::radians(d3)) * radius, sin(glm::radians(d3)) * radius) + center;

        t += 0.016f;
        KochSnowflake(p1, p2, p3, int(fmod(t, 6.0f)));


        engine->OnGUI([&]()
        {
		});
        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

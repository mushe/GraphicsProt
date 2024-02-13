#include "Core/Engine.h"


void SiepinskiTriangle(Vec2 p1, Vec2 p2, Vec2 p3, int depth)
{
    float lineWidth = 1.0f;

    if (depth == 0)
    {
        ShapeDrawer::Line(p1, p2, lineWidth);
        ShapeDrawer::Line(p2, p3, lineWidth);
        ShapeDrawer::Line(p3, p1, lineWidth);
    }
    else
    {
        auto p12 = (p1 + p2) / 2.0f;
        auto p23 = (p2 + p3) / 2.0f;
        auto p31 = (p3 + p1) / 2.0f;

        SiepinskiTriangle(p1, p12, p31, depth - 1);
        SiepinskiTriangle(p12, p2, p23, depth - 1);
        SiepinskiTriangle(p31, p23, p3, depth - 1);
    }
}


int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);


    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();


        float radius = 0.5f;
        Vec2 center = Vec2(0.5, 0.625);
        float d1 = -90.0f;
        float d2 = -210.0f;
        float d3 = -330.0f;
        auto p1 = Vec2(cos(glm::radians(d1)) * radius, sin(glm::radians(d1)) * radius) + center;
        auto p2 = Vec2(cos(glm::radians(d2)) * radius, sin(glm::radians(d2)) * radius) + center;
        auto p3 = Vec2(cos(glm::radians(d3)) * radius, sin(glm::radians(d3)) * radius) + center;

        SiepinskiTriangle(p1, p2, p3, 6);

        engine->OnGUI([&]()
        {
		});
        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

#include "SierpinskiTriangle.h"

void SierpinskiTriangle::SiepinskiTriangle(Vec2 p1, Vec2 p2, Vec2 p3, int depth)
{
    float lineWidth = 2.0f;

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

void SierpinskiTriangle::Start(shared_ptr<Engine> engine)
{
    Debug::Log("SierpinskiTriangle::Start()");
}


bool SierpinskiTriangle::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


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
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void SierpinskiTriangle::Release()
{
    Debug::Log("SierpinskiTriangle::Release()");
}

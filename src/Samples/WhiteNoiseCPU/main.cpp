#include "Core/Engine.h"

int main()
{
    auto engine = Engine::Init(800, 800, 200, 50);

    int GridNum = 70;
    int N = GridNum * GridNum;
    float GridScale = 1.0f / (float)GridNum;

    std::vector<Vec2> positions(N);
    for (int i = 0; i < N; i++)
    {
        int x = i % GridNum;
        int y = i / GridNum;
        positions[i] = Vec2(x * GridScale, y * GridScale) + Vec2(GridScale / 2.0f);
    }

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();

        for(int i=0; i < positions.size(); i++)
        {
            float r = Random::Range(0.0f, 1.0f);
            Vec4 color = Vec4(r, r, r, 1.0f);
            ShapeDrawer::Rect(positions[i], Vec2(GridScale), color);
		}

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

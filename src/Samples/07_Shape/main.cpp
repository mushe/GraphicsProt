#include "Core/Engine.h"
#include <cmath>

int main()
{
    auto engine = Engine::Init(1500, 1500, 0, 50);

    int GridNum = 20;
    int N = GridNum * GridNum;
    float GridScale = 1.0f / (float)GridNum;
    std::vector<int> randIndices(N);
    std::vector<Vec2> positions(N);
    std::vector<float> randScales(N);
    std::vector<Vec4> randColors(N);
    std::vector<Vec2> randDirections(N);
    for (int i = 0; i < N; i++)
    {
        int x = i % GridNum;
        int y = i / GridNum;

        randIndices[i] = Random::Range(0, 3);
        positions[i] = Vec2(x * GridScale, y * GridScale) + Vec2(GridScale / 2.0f);
        randScales[i] = GridScale;
        randColors[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
        randDirections[i] = Vec2(Random::Range(-1.0f, 1.0f), Random::Range(-1.0f, 1.0f));
    }

    float t = 0.0f;

    while (engine->Running())
    {
        t += 0.08f;

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();


        for(int i=0; i < randIndices.size(); i++)
        {
            float scaleMultiply = sin(t + i) * 0.5f + 0.5f;

            switch (randIndices[i])
            {
                case 0:
                    ShapeDrawer::Rect(positions[i], Vec2(randScales[i]) * scaleMultiply, randColors[i]);
					    break;
			    case 1:
                    ShapeDrawer::Circle(positions[i], Vec2(randScales[i]) * scaleMultiply, randColors[i]);
				    break;
			    case 2:
                    ShapeDrawer::Line(positions[i] - Vec2(GridScale / 2.0f) * scaleMultiply, positions[i] + Vec2(GridScale / 2.0f) * scaleMultiply, 1.0f, randColors[i]);
				    break;
                case 3:
				    ShapeDrawer::Triangle(positions[i], randDirections[i], Vec2(randScales[i]) * scaleMultiply, randColors[i]);
                    break;
			}
		}


        engine->OnGUI([&]()
        {
        });

        engine->EndRenderToScreen();

        engine->EndFrame();
    }

    engine->Terminate();
}

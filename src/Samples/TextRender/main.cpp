#include "Core/Engine.h"
#include <cmath>

using namespace std;
using namespace glm;
#include <vector>
#include <algorithm>



int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    vector<Vec2> positions;
    vector<float> scales;
    vector<Vec4> colors;
    for(int i = 0; i < 100; i++)
    {
        // random position
        positions.push_back(Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f)));
        scales.push_back(Random::Range(1.0f, 5.0f));
        colors.push_back(Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f));
    }


    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();


        for(int i = 0; i< positions.size(); i++)
        {
            Text("ABC", positions[i], scales[i], colors[i]);
        }

        engine->OnGUI([&]{
        });

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

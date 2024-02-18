#include "Core/Engine.h"
#include <cmath>

using namespace std;
using namespace glm;
#include <vector>
#include <algorithm>

vector<vec2> PoissonDiskSampling2D(float r, int k, vec2 area)
{
    vector<vec2> res;
    vector<int> activeList;

    // -----------------------------------------------
    // Step 0.
    // -----------------------------------------------

    // -----------------------------------------------
    // Step 1.
    // -----------------------------------------------

    // -----------------------------------------------
    // Step 2.
    // -----------------------------------------------

    return res;
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    float scale = 0.01f;
    vector<Vec2> positions = PoissonDiskSampling2D(0.05f, 30, vec2(1.0f, 1.0f));
    std::vector<Vec4> colors(positions.size());
    for (int i = 0; i < positions.size(); i++)
    {
        colors[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
    }


    float t = 0.0f;

    while (engine->Running())
    {
        t += 0.5f;

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        // max of positions.size() and t
        int s = std::min((int)positions.size(), (int)t);

        for(int i = 0; i < positions.size(); i++)
        {
            ShapeDrawer::Circle(positions[i], glm::vec2(scale), colors[i]);
		}

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

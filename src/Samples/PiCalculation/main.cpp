#include "Core/Engine.h"
#include <cmath>

using namespace std;
using namespace glm;
#include <vector>
#include <algorithm>

struct Point
{
    Vec2 position;
    Vec4 color;
};

float CalculatePiByPoints(vector<Point>& points)
{
    int insideCircle = 0;
    for(int i = 0; i < points.size(); i++)
    {
        if (length(points[i].position - Vec2(0.5f, 0.5f)) < 0.5f)
        {
             insideCircle++;
        }
        else
        {
            points[i].color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    return 4.0f * (float)insideCircle / (float)points.size();
}

Point GenerateRandomPoint()
{
    return { Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f)), Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f) };
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    vector<Point> points;

    float t = 0.0f;
    float interval = 0.016f;
    float scale = 0.01f;

    while (engine->Running())
    {
        t += 0.016f;
        if (t > interval)
        {
            t = 0.0f;
            points.push_back(GenerateRandomPoint());
        }

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        for(int i = 0; i < points.size(); i++)
        {
            ShapeDrawer::Circle(points[i].position, glm::vec2(scale), points[i].color);
		}

        float estimatePi = CalculatePiByPoints(points);

        Text("Estimate PI = " + to_string(estimatePi), Vec2(0.2f, 0.5f), 2.0f);

        engine->OnGUI([&]()
        {
            GUI::Label("Estimate Pi: " + to_string(estimatePi));
            GUI::Label(to_string(points.size()) + "points");
            GUI::Float(interval, "interval", 0.01f);
            GUI::Float(scale, "scale", 0.001f);
		});

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

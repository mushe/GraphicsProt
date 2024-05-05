#include "PiCalculation.h"

float PiCalculation::CalculatePiByPoints(vector<Point>& points)
{
    int insideCircle = 0;
    for (int i = 0; i < points.size(); i++)
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

Point PiCalculation::GenerateRandomPoint()
{
    return { Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f)), Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f) };
}

void PiCalculation::Start(shared_ptr<Engine> engine)
{
}


bool PiCalculation::Update(shared_ptr<Engine> engine)
{
    t_ += 0.016f;
    if (t_ > interval_)
    {
        t_ = 0.0f;
        points_.push_back(GenerateRandomPoint());
    }

    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    for (int i = 0; i < points_.size(); i++)
    {
        ShapeDrawer::Circle(points_[i].position, glm::vec2(scale_), points_[i].color);
    }

    float estimatePi = CalculatePiByPoints(points_);

    Text("Estimate PI = " + to_string(estimatePi), Vec2(0.2f, 0.5f), 2.0f);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Label("Estimate Pi: " + to_string(estimatePi));
        GUI::Label(to_string(points_.size()) + "points");
        GUI::Float(interval_, "interval", 0.01f);
        GUI::Float(scale_, "scale", 0.001f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void PiCalculation::Release()
{
}

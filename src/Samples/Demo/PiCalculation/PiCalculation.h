#pragma once
#include "Core/Scene.h"

struct Point
{
    Vec2 position;
    Vec4 color;
};

class PiCalculation : public Scene
{
public:
    PiCalculation():
        t_(0.0f),
        interval_(0.016f),
        scale_(0.01f)
    {
        Debug::Log("PiCalculation()");
    }
    virtual ~PiCalculation() { Debug::Log("~PiCalculation()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    vector<Point> points_;
    float t_;
    float interval_;
    float scale_;

    float CalculatePiByPoints(vector<Point>& points);
    Point GenerateRandomPoint();
};
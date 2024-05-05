#pragma once
#include "Core/Scene.h"


class CatmullRomSpline : public Scene
{
public:
    CatmullRomSpline():
		t_(0.0f)
    { }
    virtual ~CatmullRomSpline() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    Vec2 CatmullRomSplineInterp(Vec2 p_mi1, Vec2 p_0, Vec2 p_1, Vec2 p_2, float t);

    float t_;
};
#pragma once
#include "Core/Scene.h"


class KochSnowflake : public Scene
{
public:
    KochSnowflake():
        t_(1.0f)
    {
        Debug::Log("KochSnowflake()");
    }
    virtual ~KochSnowflake() { Debug::Log("~KochSnowflake()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    float t_;

    void KochCurve(Vec2 start, Vec2 end, int depth);
    void Snowflake(Vec2 p1, Vec2 p2, Vec2 p3, int depth);
};
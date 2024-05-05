#pragma once
#include "Core/Scene.h"


class SierpinskiTriangle : public Scene
{
public:
    SierpinskiTriangle()
    {
    }
    virtual ~SierpinskiTriangle() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    void SiepinskiTriangle(Vec2 p1, Vec2 p2, Vec2 p3, int depth);
};
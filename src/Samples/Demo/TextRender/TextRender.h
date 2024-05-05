#pragma once
#include "Core/Scene.h"


class TextRender : public Scene
{
public:
    TextRender():
        t_(0.0f)
    {
    }
    virtual ~TextRender() { }

    void Reset();
    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    vector<Vec2> positions_;
    vector<float> scales_;
    vector<Vec4> colors_;
    float t_;
};
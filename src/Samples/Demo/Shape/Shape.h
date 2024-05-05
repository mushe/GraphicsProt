#pragma once
#include "Core/Scene.h"


class Shape : public Scene
{
public:
    Shape():
        GridNum_(20),
        N_(GridNum_ * GridNum_),
        GridScale_(1.0f / (float)GridNum_),
        t_(0.0f)
    {
    }
    virtual ~Shape() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    int GridNum_;
    int N_;
    float GridScale_;
    float t_;
    std::vector<int> randIndices_;
    std::vector<Vec2> positions_;
    std::vector<float> randScales_;
    std::vector<Vec4> randColors_;
    std::vector<Vec2> randDirections_;
};
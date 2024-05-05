#pragma once
#include "Core/Scene.h"


class WhiteNoiseCPU : public Scene
{
public:
    WhiteNoiseCPU():
        GridNum_(70),
        N_(GridNum_* GridNum_),
        GridScale_(1.0f / (float)GridNum_)
    {
    }
    virtual ~WhiteNoiseCPU() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    int GridNum_;
    int N_;
    float GridScale_;
    vector<Vec2> positions_;
};
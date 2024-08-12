#pragma once
#include "Core/Scene.h"


class Eratosthenes : public Scene
{
public:
    Eratosthenes()
    {
    }
    virtual ~Eratosthenes() 
    { 
    }

    struct NumberObject
    {
        int number_ = 0;
        Vec2 position_ = Vec2(0, 0);
        Vec4 bgColor_ = Vec4(1,1,1,1);
    };

    void Reset();
    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    void CalcStepByStep();

private:
    vector<NumberObject> numberObjects_;

    const int col_ = 15;
    const int maxNumber_ = 240;

    int currentMultipleBase_ = 0;
    int currentMultiple_ = 0;
};
#include "Eratosthenes.h"
#include <thread>
#include <iostream>
#include <chrono>


void Eratosthenes::Reset()
{
    numberObjects_.clear();

    for(int i = 1; i <= maxNumber_; i++)
    {
        NumberObject obj;
        obj.number_ = i;
        obj.position_ = Vec2
        (
            0.07f + 0.06f * ((i-1) % col_), 
            0.22f + 0.05f * ((i-1) / col_)
        );
        obj.bgColor_ = Vec4(0.025f, 0.025f, 0.025f, 1.0f);
        numberObjects_.push_back(obj);
    }
}


// Sieve of Eratosthenes one by one 
void Eratosthenes::CalcStepByStep()
{
    auto sleep = [](float sec)->void
    {
        std::chrono::duration<float> duration(sec);
        std::this_thread::sleep_for(duration);
    };

    sleep(1.0f);
    numberObjects_[0].bgColor_ = Vec4(1.0f);

    std::function<void()> func = [&]
    {
        for(int base = 2; base <= maxNumber_; base++)
        {
            for(int multiple = base * 2; multiple <= maxNumber_; multiple += base)
            {
                numberObjects_[multiple-1].bgColor_ = Vec4(1.0f);
                currentMultipleBase_ = base;
                currentMultiple_ = multiple;
                sleep(0.13f);
            }
        }

        currentMultipleBase_ = 9999;
        currentMultiple_ = 9999;

    };

    std::thread t(func);
    t.detach();
}


void Eratosthenes::Start(shared_ptr<Engine> engine)
{
    Reset();

    CalcStepByStep();
}


bool Eratosthenes::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    bool finished = currentMultiple_ == 9999;

    for(auto obj : numberObjects_)
    {
        Text(std::to_string(obj.number_), obj.position_, 1.0f, Vec4(1,1,1,1));

        Vec4 col = obj.bgColor_;

        if(!finished)
        {
            if(obj.number_ == currentMultipleBase_) col = Vec4(0.2f, 0.0f, 0.0f, 1.0f);
            else if(obj.number_ == currentMultiple_) col = Vec4(0.0f, 0.0f, 0.6f, 1.0f);
        }


        ShapeDrawer::Rect(obj.position_ + Vec2(0.015f, 0.0f), Vec2(0.055f, 0.03f), col);
    }

    if(finished)
    {
        Text("Finished!", Vec2(0.5f, 0.1f), 2.0f);
    }
    else
    {
        Text("exclude multiple of " + to_string(currentMultipleBase_), Vec2(0.35f, 0.1f), 1.8f);

    }

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Eratosthenes::Release()
{
    numberObjects_.clear();
}

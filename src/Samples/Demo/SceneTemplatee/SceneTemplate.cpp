#include "SceneTemplate.h"


void SceneTemplate::Start(shared_ptr<Engine> engine)
{
    Debug::Log("GerstnerWaves::Start()");
}


bool SceneTemplate::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    engine->OnGUI([&]()
    {
        if (GUI::Button("Back To Menu")) closeScene = true;
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void SceneTemplate::Release()
{
    Debug::Log("Boids2dCPU::Release()");
}

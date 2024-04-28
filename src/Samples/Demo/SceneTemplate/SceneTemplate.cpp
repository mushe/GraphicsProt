#include "SceneTemplate.h"


void SceneTemplate::Start(shared_ptr<Engine> engine)
{
    Debug::Log("SceneTemplate::Start()");
}


bool SceneTemplate::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void SceneTemplate::Release()
{
    Debug::Log("SceneTemplate::Release()");
}

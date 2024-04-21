#include "Core/Engine.h"
#include "Core/SceneManager.h"

#include "GerstnerWaves/GerstnerWaves.h"

int main()
{
    // engine initialization
    auto engine = Engine::Init();

    SceneManager manager(engine);
    auto gerstnerWavesScene = make_shared<GerstnerWaves>();


    // main rendering loop
    while (engine->Running())
    {
        if(manager.UpdateCurrentScene())
            continue;

        engine->BeginFrame(); // must be called at the beginning of every frame

        // input handling
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        // object rendering to screen by camera
        engine->BeginRenderToScreen();

        engine->OnGUI([&]()
        {
            GUI::Label("select a scene to open");
            GUI::Space();
            if(GUI::Button("Gerstner Waves")) manager.OpenScene(gerstnerWavesScene);
		});

        engine->EndRenderToScreen();
        engine->EndFrame(); // must be called at the end of every frame
    }

    gerstnerWavesScene->Release();
    engine->Terminate();
}

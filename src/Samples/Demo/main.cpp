#include "Core/Engine.h"
#include "Core/SceneManager.h"

#include "GerstnerWaves/GerstnerWaves.h"
#include "Boids2dCPU/Boids2dCPU.h"
#include "PerlinNoise/PerlinNoise.h"
#include "KiraSticker/KiraSticker.h"

int main()
{
    // engine initialization
    auto engine = Engine::Init();

    SceneManager manager(engine);
    auto gerstnerWavesScene = make_shared<GerstnerWaves>();
    auto boids2dScene = make_shared<Boids2dCPU>();
    auto perlinNoiseScene = make_shared<PerlinNoise>();
    auto kiraStickerScene = make_shared<KiraSticker>();


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
            GUI::Space();
            GUI::Label("select a scene to open");
            GUI::Space();
            if(GUI::Button(" ocean wave generation (gerstner waves) ")) manager.OpenScene(gerstnerWavesScene);
            GUI::SameLine();
            if(GUI::Button(" flocking simulation (boids 2d cpu) ")) manager.OpenScene(boids2dScene);

            GUI::Space();
            if (GUI::Button(" gpu noise generation (perlin noise) ")) manager.OpenScene(perlinNoiseScene);
            GUI::SameLine();
            if (GUI::Button(" old style kirakira sticker ")) manager.OpenScene(kiraStickerScene);
		});

        engine->EndRenderToScreen();
        engine->EndFrame(); // must be called at the end of every frame
    }

    gerstnerWavesScene->Release();
    engine->Terminate();
}

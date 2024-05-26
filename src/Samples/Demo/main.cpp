#include "Core/Engine.h"
#include "Core/SceneManager.h"

#include "GerstnerWaves/GerstnerWaves.h"
#include "Boids2dCPU/Boids2dCPU.h"
#include "PerlinNoise/PerlinNoise.h"
#include "KiraSticker/KiraSticker.h"
#include "Minimum/Minimum.h"
#include "GrayScale/GrayScale.h"
#include "Sepia/Sepia.h"
#include "ChromaticAberration/ChromaticAberration.h"
#include "Kaleidoscope/Kaleidoscope.h"
#include "VHS/VHS.h"
#include "CatmullRomSpline/CatmullRomSpline.h"
#include "PoissonDiskSampling/PoissonDiskSampling.h"
#include "Grass/Grass.h"
#include "Maze/Maze.h"
#include "Instancing/Instancing.h"
#include "Hover/Hover.h"
#include "NormalMapping/NormalMapping.h"
#include "PseudoColor/PseudoColor.h"
#include "Posterization/Posterization.h"
#include "River/River.h"
#include "Shape/Shape.h"
#include "TextRender/TextRender.h"
#include "WhiteNoiseGPU/WhiteNoiseGPU.h"
#include "HalfTone/HalfTone.h"
#include "DungeonGeneration/DungeonGeneration.h"
#include "Dungeon2D/Dungeon2D.h"
#include "Dungeon3D/Dungeon3D.h"
#include "KochSnowflake/KochSnowflake.h"
#include "PiCalculation/PiCalculation.h"
#include "RoomDivision/RoomDivision.h"
#include "SierpinskiTriangle/SierpinskiTriangle.h"
#include "WhiteNoiseCPU/WhiteNoiseCPU.h"
#include "ComputeShader/ComputeShaderScene.h"
#include "Blur/Blur.h"
#include "FluidSim/FluidSim.h"

int main()
{
    // engine initialization
    auto engine = Engine::Init(800, 800);
    engine->ChangeWindowTitle("scene selection");

    SceneManager manager(engine);
    auto gerstnerWavesScene = make_shared<GerstnerWaves>();
    auto boids2dScene = make_shared<Boids2dCPU>();
    auto perlinNoiseScene = make_shared<PerlinNoise>();
    auto kiraStickerScene = make_shared<KiraSticker>();
    auto minimumScene = make_shared<Minimum>();
    auto grayScaleScene = make_shared<GrayScale>();
    auto sepiaScaleScene = make_shared<Sepia>();
    auto chromaticAberrationScene = make_shared<ChromaticAberration>();
    auto kaleidoscopeScene = make_shared<Kaleidoscope>();
    auto vhsScene = make_shared<VHS>();
    auto catmullRomSplineScene = make_shared<CatmullRomSpline>();
    auto poissonDiskSamplingScene = make_shared<PoissonDiskSampling>();
    auto grassScene = make_shared<Grass>();
    auto mazeScene = make_shared<Maze>();
    auto instancingScene = make_shared<Instancing>();
    auto hoverScene = make_shared<Hover>();
    auto normalMappingScene = make_shared<NormalMapping>();
    auto pseudoColorScene = make_shared<PseudoColor>();
    auto posterizationScene = make_shared<Posterization>();
    auto riverScene = make_shared<River>();
    auto shapeScene = make_shared<Shape>();
    auto textRenderScene = make_shared<TextRender>();
    auto whiteNoiseGPUScene = make_shared<WhiteNoiseGPU>();
    auto halfToneScene = make_shared<HalfTone>();
    auto dungeonGenerationScene = make_shared<DungeonGeneration>();
    auto dungeon2DScene = make_shared<Dungeon2D>();
    auto dungeon3DScene = make_shared<Dungeon3D>();
    auto kochSnowflakeScene = make_shared<KochSnowflake>();
    auto piCalculationScene = make_shared<PiCalculation>();
    auto roomDivisionScene = make_shared<RoomDivision>();
    auto sierpinskiTriangleScene = make_shared<SierpinskiTriangle>();
    auto whiteNoiseCPUScene = make_shared<WhiteNoiseCPU>();
    auto computeShaderScene = make_shared<ComputeShaderScene>();
    auto blurScene = make_shared<Blur>();
    auto fluidSimScene = make_shared<FluidSim>();
    
    // main rendering loop
    while (engine->Running())
    {
        if(manager.UpdateCurrentScene())
            continue;

        engine->BeginFrame(); // must be called at the beginning of every frame

        // input handling
        if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();

        // object rendering to screen by camera
        engine->BeginRenderToScreen();

        engine->OnGUI([&]()
        {
            rep(i,3) GUI::Space();
            GUI::Label("select a scene to open");
            rep(i,3) GUI::Space();
            if(GUI::Button(" ocean wave generation (gerstner waves) ")) manager.OpenScene(gerstnerWavesScene, "ocean wave generation (gerstner waves)");
            GUI::SameLine();
            if(GUI::Button(" flocking simulation (boids 2d cpu) ")) manager.OpenScene(boids2dScene, "flocking simulation (boids 2d cpu)");

            GUI::Space();
            if (GUI::Button(" river ")) manager.OpenScene(riverScene, "river");
            GUI::SameLine();
            if (GUI::Button(" grass ")) manager.OpenScene(grassScene, "grass");
            GUI::SameLine();
            if (GUI::Button(" gpu noise generation (perlin noise) ")) manager.OpenScene(perlinNoiseScene, "gpu noise generation (perlin noise)");
            GUI::SameLine();
            if (GUI::Button(" old style kirakira sticker ")) manager.OpenScene(kiraStickerScene, "old style kirakira sticker");

            GUI::Space();
            if (GUI::Button(" minimum scene (render 3d object) ")) manager.OpenScene(minimumScene, "minimum scene (render 3d object)");
            GUI::SameLine();
            if (GUI::Button(" instancing ")) manager.OpenScene(instancingScene, "instancing");
            GUI::SameLine();
            if (GUI::Button(" catmull-rom spline ")) manager.OpenScene(catmullRomSplineScene, "catmull-rom spline");
            GUI::SameLine();
            if (GUI::Button(" hover game ")) manager.OpenScene(hoverScene, "hover game");


            GUI::Space();
            if (GUI::Button(" poisson disk sampling ")) manager.OpenScene(poissonDiskSamplingScene, "poisson disk sampling");
            GUI::SameLine();
            if (GUI::Button(" maze generation ")) manager.OpenScene(mazeScene, "maze generation");
            GUI::SameLine();
            if (GUI::Button(" normal mapping ")) manager.OpenScene(normalMappingScene, "normal mapping");
            GUI::SameLine();
            if (GUI::Button(" pseudo color ")) manager.OpenScene(pseudoColorScene, "pseudo color");
            GUI::SameLine();
            if (GUI::Button(" posterization ")) manager.OpenScene(posterizationScene, "posterization");

            GUI::Space();
            if (GUI::Button(" shape rendering ")) manager.OpenScene(shapeScene, "shape rendering");
            GUI::SameLine();
            if (GUI::Button(" text rendering ")) manager.OpenScene(textRenderScene, "text rendering");
            GUI::SameLine();
            if (GUI::Button(" gpu white noise ")) manager.OpenScene(whiteNoiseGPUScene, "gpu white noise");
            GUI::SameLine();
            if (GUI::Button(" cpu white noise ")) manager.OpenScene(whiteNoiseCPUScene, "cpu white noise");

            GUI::Space();
            if (GUI::Button(" room division ")) manager.OpenScene(roomDivisionScene, "room division");
            GUI::SameLine();
            if (GUI::Button(" dungeon generation ")) manager.OpenScene(dungeonGenerationScene, "dungeon generation");
            GUI::SameLine();
            if (GUI::Button(" 2d dungeon exploring ")) manager.OpenScene(dungeon2DScene, "2d dungeon exploring");
            GUI::SameLine();
            if (GUI::Button(" 3d dungeon exploring ")) manager.OpenScene(dungeon3DScene, "3d dungeon exploring");

            GUI::Space();
            if (GUI::Button(" koch snowflake ")) manager.OpenScene(kochSnowflakeScene, "koch snowflake");
            GUI::SameLine();
            if (GUI::Button(" sierpinski triangle ")) manager.OpenScene(sierpinskiTriangleScene, "sierpinski triangle");
            GUI::SameLine();
            if (GUI::Button(" pi estimation ")) manager.OpenScene(piCalculationScene, "pi estimation");
            GUI::SameLine();
            if (GUI::Button(" gray scale (post process) ")) manager.OpenScene(grayScaleScene, "gray scale (post process)");

            GUI::Space();
            if (GUI::Button(" vhs (post process) ")) manager.OpenScene(vhsScene, "vhs (post process)");
            GUI::SameLine();
            if (GUI::Button(" sepia (post process) ")) manager.OpenScene(sepiaScaleScene, "sepia (post process)");
            GUI::SameLine();
            if (GUI::Button(" chromatic aberration (post process) ")) manager.OpenScene(chromaticAberrationScene, "chromatic aberration (post process)");
            
            GUI::Space();
            if (GUI::Button(" halftone (post process) ")) manager.OpenScene(halfToneScene, "halftone (post process)");
            GUI::SameLine();
            if (GUI::Button(" kaleidoscope (post process) ")) manager.OpenScene(kaleidoscopeScene, "kaleidoscope (post process)");
            GUI::SameLine();
            if (GUI::Button(" compute shader (fish swarm) ")) manager.OpenScene(computeShaderScene, "compute shader (fish swarm)");

            GUI::Space();
            if (GUI::Button(" blur (post process) ")) manager.OpenScene(blurScene, "blur (post process)");
            GUI::SameLine();
            if (GUI::Button(" fluid simulation ")) manager.OpenScene(fluidSimScene, "fluid simulation");

            rep(i,5) GUI::Space();
            if (GUI::Button(" Exit ")) engine->Quit();

            rep(i, 15) GUI::Space();

		});

        engine->EndRenderToScreen();
        engine->EndFrame(); // must be called at the end of every frame
    }

    engine->Terminate();
}

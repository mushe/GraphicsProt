#include "Boids2dCPU.h"


void Boids2dCPU::Start(shared_ptr<Engine> engine)
{
    boids.resize(boidsCount);
    for (int i = 0; i < boidsCount; i++)
    {
        boids[i].pos = Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f));
        boids[i].vel = Vec2(Random::Range(-1.0f, 1.0f), Random::Range(-1.0f, 1.0f));
        boids[i].color = Vec4(0.0f, 0.6f, 1.0f, 1.0f);
    }
}


bool Boids2dCPU::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();


    // simulation 
    for (int i = 0; i < boidsCount; i++)
    {
        Vec2 acc = Vec2(0.0f);

        Vec2 alignment = Vec2(0.0f);
        Vec2 cohesion = Vec2(0.0f);
        Vec2 separation = Vec2(0.0f);
        int alignmentCount = 0;
        int cohesionCount = 0;
        int separationCount = 0;
        for (int j = 0; j < boidsCount; j++)
        {
            if (i == j) continue;

            Vec2 diff = boids[i].pos - boids[j].pos;
            float d = length(diff);
            if (d < alignmentRadius)
            {
                alignment += boids[j].vel;
                alignmentCount++;
            }
            if (d < cohesionRadius)
            {
                cohesion += boids[j].pos;
                cohesionCount++;
            }
            if (d < separationRadius)
            {
                separation += diff;
                separationCount++;
            }
        }
        if (alignmentCount > 0)
        {
            alignment /= (float)alignmentCount;
            acc += alignment * alignmentWeight;
        }
        if (cohesionCount > 0)
        {
            cohesion /= (float)cohesionCount;
            acc += (cohesion - boids[i].pos) * cohesionWeight;
        }
        if (separationCount > 0)
        {
            separation /= (float)separationCount;
            acc += separation * separationWeight;
        }


        if (boids[i].pos.x < 0.0f) boids[i].pos.x = 1.0f;
        if (boids[i].pos.x > 1.0f) boids[i].pos.x = 0.0f;
        if (boids[i].pos.y < 0.0f) boids[i].pos.y = 1.0f;
        if (boids[i].pos.y > 1.0f) boids[i].pos.y = 0.0f;

        boids[i].vel += acc;
        // truncate velocity
        if (length(boids[i].vel) > maxVelocity)
            boids[i].vel = normalize(boids[i].vel) * maxVelocity;

        boids[i].pos += boids[i].vel * 0.01f;
    }

    bool closeScene = false;


    engine->BeginRenderToScreen();

    // display
    for (int i = 0; i < boidsCount; i++)
    {
        ShapeDrawer::Triangle(boids[i].pos, boids[i].vel, Vec2(boidScale * 0.05f), boids[i].color);
    }

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Boids2dCPU::Release()
{
}

#include "Core/Engine.h"

struct Particle
{
    Vec2 position = Vec2(0.5f, 0.5f);
    float lifeTime = 1.0f;
    float age = 0.0f;
};

Particle SpawnParticle(Vec2 position)
{
    Particle particle;
    particle.position = position;
    particle.lifeTime = 1.0f;
    particle.age = 0.0f;
    return particle;
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    // parameters
    float gravity = -9.8f;
    Vec2 initialPosition = Vec2(0.5f, 0.75f);
    float initialVelocity = 200.0f;
    float hoverForce = 8.0f;
    float rotationalForce = 40.0f;
    float rotationalVelocityAttenuation = 0.95f;
    float particleSpawnRate = 1.0f;

    // variables
    Vec2 position = initialPosition;
    float rotation = 0.0f;
    float velocity = initialVelocity * 0.0001;;
    float rotationalVelocity = 0.0f;

    // jet particle
    std::vector<Vec2> particles;

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();
        engine->BeginRenderToScreen();

        // input
        float leftInput = 0.0f;
        float rightInput = 0.0f;
        if(Input::KeyDown(KeyCode::LeftShift))
            leftInput += 1.0f;
        if(Input::KeyDown(KeyCode::RightShift))
            rightInput += 1.0f;

        // calculatr position
        float hover = (leftInput + rightInput) * hoverForce;
        float rot = (rightInput - leftInput) * rotationalForce;
        velocity += gravity * 0.0001f + hover * 0.0001f;
        position.y += velocity;
        rotationalVelocity += rot * 0.0001f;
        rotationalVelocity *= rotationalVelocityAttenuation;  
        rotation += rotationalVelocity;

        // reset
        if(length(position - Vec2(0.5f)) > 0.9)
        {
            position = initialPosition;
            rotation = 0.0f;
            velocity = initialVelocity * 0.0001;;
            rotationalVelocity = 0.0f;
        }

        // display
        float bodyLength = 0.25f;
        Vec2 start = Vec2(cos(-rotation) * bodyLength, sin(-rotation) * bodyLength) + Vec2(position.x, 1.0f-position.y);
        Vec2 end = Vec2(-cos(-rotation) * bodyLength, -sin(-rotation) * bodyLength) + Vec2(position.x, 1.0f-position.y);
        ShapeDrawer::Line(start, end, 50.0f, Vec4(0.431, 0.561, 0.859, 1.0));
        Vec4 jetColor = Vec4(0.6, 0.4, 0.3, 1.0);
        ShapeDrawer::Triangle(start, Vec2(0, -1.0f), Vec2(0.1f, 0.05f), jetColor);
        ShapeDrawer::Triangle(end, Vec2(0, -1.0f), Vec2(0.1f, 0.05f), jetColor);

        
        // particles
        int particleAddCount = particleSpawnRate * (leftInput + rightInput);
        for(int i = 0; i < particleAddCount; i++)
        {
            particles.push_back(Vec2(0.5f, 0.5f));
        }
        for(auto particle : particles)
        {
            ShapeDrawer::Rect(particle, Vec2(0.2f), Vec4(0.6, 0.4, 0.3, 1.0));
        }

        // ui 
        Text("particle count : " + to_string(particles.size()), Vec2(0.05, 0.65));
        Text("press LEFT SHIFT and RIGHT SHIFT key to hover", Vec2(0.05, 0.7));
        Text("left and right input : " + to_string(int(leftInput)) + "," + to_string(int(rightInput)), Vec2(0.05, 0.75));

        Text("position : " + to_string(position.x) + "," + to_string(position.y), Vec2(0.05, 0.80));
        Text("velocity : " + to_string(velocity) , Vec2(0.05, 0.85));
        Text("rotation : " + to_string(rotation), Vec2(0.05, 0.9));
        Text("rotationalVelocity : " + to_string(rotationalVelocity), Vec2(0.05, 0.95));
        engine->OnGUI([&]()
        {
            GUI::Float(hoverForce, "hover force", 0.1f);
            GUI::Float(gravity, "gravity", 0.1f);
            GUI::Float(initialVelocity, "initial velocity", 0.1f);
		});

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

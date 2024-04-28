#include "Hover.h"

Particle Hover::SpawnParticle(Vec2 position)
{
    Particle particle;

    float posXVariation = 0.03f;
    float posYVariation = 0.01f;
    particle.position = position + Vec2(Random::Range(-posXVariation, posXVariation), Random::Range(-posYVariation, posYVariation));

    float velXVariation = 0.005f;
    float velY = 0.01f + Random::Range(0.0f, 0.005f);
    particle.velocity = Vec2(Random::Range(-velXVariation, velXVariation), velY);

    particle.lifeTime = 1.0f;
    particle.age = 0.0f;
    particle.scale = Random::Range(0.005f, 0.01f);
    return particle;
}

void Hover::Start(shared_ptr<Engine> engine)
{
    Debug::Log("Hover::Start()");
}


bool Hover::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;



    // input
    float leftInput = 0.0f;
    float rightInput = 0.0f;
    if (Input::KeyDown(InputCode::Key::LeftShift))
        leftInput += 1.0f;
    if (Input::KeyDown(InputCode::Key::RightShift))
        rightInput += 1.0f;

    // calculatr position
    float hover = (leftInput + rightInput) * hoverForce_;
    float rot = (rightInput - leftInput) * rotationalForce_;
    velocity_ += gravity_ * 0.0001f + hover * 0.0001f;
    position_.y += velocity_;
    rotationalVelocity_ += rot * 0.0001f;
    rotationalVelocity_ *= rotationalVelocityAttenuation_;
    rotation_ += rotationalVelocity_;

    // reset
    if (length(position_ - Vec2(0.5f)) > 0.9)
    {
        position_ = initialPosition_;
        rotation_ = 0.0f;
        velocity_ = initialVelocity_ * 0.0001;;
        rotationalVelocity_ = 0.0f;
    }


    engine->BeginRenderToScreen();

    // display
    float bodyLength = 0.25f;
    Vec2 start = Vec2(cos(-rotation_) * bodyLength, sin(-rotation_) * bodyLength) + Vec2(position_.x, 1.0f - position_.y);
    Vec2 end = Vec2(-cos(-rotation_) * bodyLength, -sin(-rotation_) * bodyLength) + Vec2(position_.x, 1.0f - position_.y);
    ShapeDrawer::Line(start, end, 50.0f, Vec4(0.431, 0.561, 0.859, 1.0));
    Vec4 jetColor = Vec4(0.6, 0.4, 0.3, 1.0);
    ShapeDrawer::Triangle(start, Vec2(0, -1.0f), Vec2(0.1f, 0.05f), jetColor);
    ShapeDrawer::Triangle(end, Vec2(0, -1.0f), Vec2(0.1f, 0.05f), jetColor);


    // particles
    int leftParticleAddCount = particleSpawnRate_ * leftInput;
    int rightParticleAddCount = particleSpawnRate_ * rightInput;
    for (int i = 0; i < leftParticleAddCount; i++)
    {
        particles_.push_back(SpawnParticle((start.x > end.x) ? end : start));
    }
    for (int i = 0; i < rightParticleAddCount; i++)
    {
        particles_.push_back(SpawnParticle((start.x > end.x) ? start : end));
    }
    for (auto& particle : particles_)
    {
        particle.position += particle.velocity;
        ShapeDrawer::Rect(particle.position, Vec2(particle.scale), Vec4(0.6, 0.4, 0.3, 1.0));

        // lifetime calculation
        particle.age += 0.016f;
    }

    particles_.erase(std::remove_if(particles_.begin(), particles_.end(), [](Particle p) {
        return p.age > p.lifeTime;
    }), particles_.end());

    // ui 
    Text("press LEFT SHIFT or RIGHT SHIFT key to hover", Vec2(0.18, 0.03));
    Text("particle count : " + to_string(particles_.size()), Vec2(0.05, 0.7));
    Text("left and right input : " + to_string(int(leftInput)) + "," + to_string(int(rightInput)), Vec2(0.05, 0.75));

    Text("position : " + to_string(position_.x) + "," + to_string(position_.y), Vec2(0.05, 0.80));
    Text("velocity : " + to_string(velocity_), Vec2(0.05, 0.85));
    Text("rotation : " + to_string(rotation_), Vec2(0.05, 0.9));
    Text("rotationalVelocity : " + to_string(rotationalVelocity_), Vec2(0.05, 0.95));

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Float(hoverForce_, "hover force", 0.1f);
        GUI::Float(gravity_, "gravity", 0.1f);
        GUI::Float(initialVelocity_, "initial velocity", 0.1f);
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Hover::Release()
{
    Debug::Log("Hover::Release()");
}

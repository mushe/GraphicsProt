#include "Core/Engine.h"

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    // parameters
    float gravity = -9.8f;
    float initialPosition = 0.25f;
    float initialVelocity = 200.0f;
    float hoverForce = 10.0f;

    // variables
    float position = initialPosition;
    float velocity = initialVelocity * 0.0001;;

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

        float hover = (leftInput + rightInput) * hoverForce;
        
        // calculatr position
        velocity += gravity * 0.0001 + hover * 0.0001f;
        position += velocity;
        if(position < -0.6)
        {
            position = initialPosition;
            velocity = initialVelocity * 0.0001;;
        }

        // display
        ShapeDrawer::Line(Vec2(0.25, 0.5 - position), Vec2(0.75, 0.5 - position), 50.0f, Vec4(0.431, 0.561, 0.859, 1.0));

        // ui 
        Text("press LEFT SHIFT and RIGHT SHIFT key to hover", Vec2(0.05, 0.85));
        Text("position : " + to_string(position), Vec2(0.05, 0.9));
        Text("left and right input : " + to_string(int(leftInput)) + "," + to_string(int(rightInput)), Vec2(0.05, 0.95));
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

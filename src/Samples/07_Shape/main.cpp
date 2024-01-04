#include "Core/Engine.h"

struct CustomUBO :public UniformBufferBase
{
    float scale = 1.0f;
};

struct InstancingBuffer : InstancingUniformBufferBase
{
    glm::vec2 position = glm::vec2(0);
    glm::vec2 scale = glm::vec2(0);
};



int main()
{
    auto engine = Engine::Init(1500,1500, 1200, 500);

    std::vector<glm::vec2> positions(2000);
    for (int i = 0; i < positions.size(); ++i)
    {
		positions[i] = glm::vec2(Random::Range(0.01f, 1.0f), Random::Range(0.01f, 1.0f));
	}

    float t = 0.0f;

    while (engine->Running())
    {
        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();
        t += 0.016f;
        
        for (auto& p : positions)
        {
			ShapeDrawer::Rect(p, glm::vec2(0.01));
		}   

        for (int i = 0; i < 100; ++i)
        {
            auto p = glm::vec2
            (
                i * 0.01,
                sin(i * 0.1 + t * 0.5f) * 0.3 + 0.5
            );

            ShapeDrawer::Rect(p, glm::vec2(0.01));
        }

        engine->EndRenderToScreen();


        engine->OnGUI([&]()
        {
        });
        engine->EndFrame();
    }

    engine->Terminate();
}

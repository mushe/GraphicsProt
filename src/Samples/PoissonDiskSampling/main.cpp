#include "Core/Engine.h"
#include <cmath>

using namespace std;
using namespace glm;
#include <vector>
#include <algorithm>

vector<vec2> PoissonDiskSampling2D(float r, int k, vec2 area)
{
    vector<vec2> res;
    vector<int> activeList;

    // -----------------------------------------------
    // Step 0.
    // -----------------------------------------------
    float cellSize = r / glm::sqrt(2.0f);

    int cols = round(area.x / cellSize) + 1;
    int rows = round(area.y / cellSize) + 1;

    vector<vector<int>> grid;
    for (int i = 0; i < rows; i++)
    {
        vector<int> arr;
        for (int j = 0; j < cols; j++)
        {
            arr.push_back(-1);
        }
        grid.push_back(arr);
    }

    // -----------------------------------------------
    // Step 1.
    // -----------------------------------------------
    float initialX = Random::Range(0.0f, area.x);
    float initialY = Random::Range(0.0f, area.y);
    vec2 initialPosition = vec2(initialX, initialY);

    int initialXIndex = round(initialX / cellSize);
    int initialYIndex = round(initialY / cellSize);
    grid[initialYIndex][initialXIndex] = 0;

    activeList.push_back(0);
    res.push_back(initialPosition);

    // -----------------------------------------------
    // Step 2.
    // -----------------------------------------------
    int currentIndex = 0;
    while (activeList.size() > 0)
    {
        auto activeIndex = activeList[Random::Range(0, activeList.size() - 1)];
        auto activePos = res[activeIndex];

        bool found = false;
        for (int i = 0; i < k; i++)
        {
            float radian = Random::Range(0.0f, 2.0f * PI);
            float radius = Random::Range(r, r * 2.0f);
            vec2 p = vec2(cos(radian), sin(radian)) * radius + activePos;

            int xIndex = round(p.x / cellSize);
            int yIndex = round(p.y / cellSize);
            if (xIndex < 0 || yIndex <0 || xIndex > cols - 1 || yIndex > rows - 1)
            {
                continue;
            }

            if (grid[yIndex][xIndex] == -1)
            {
                bool farFromAll = true;
                for (int j = -1; j <= 1; j++)
                {
                    for (int k = -1; k <= 1; k++)
                    {
                        int checkXIndex = xIndex + k;
                        int checkYIndex = yIndex + j;
                        if (checkXIndex < 0 || checkYIndex <0 || checkXIndex > cols - 1 || checkYIndex > rows - 1) continue;

                        int checkIndex = grid[checkYIndex][checkXIndex];
                        if (checkIndex == -1) continue;

                        vec2 targetPos = res[checkIndex];
                        if ((targetPos - p).length() < r) farFromAll = false;
                    }
                }

                if (farFromAll)
                {
                    currentIndex++;
                    grid[yIndex][xIndex] = currentIndex;
                    activeList.push_back(currentIndex);
                    res.push_back(p);
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            int index = -1;
            for (int i = 0; i < activeList.size(); i++)
            {
                if (activeList[i] == activeIndex)
                {
                    index = i;
                    break;
                }
            }
            activeList.erase(activeList.begin() + index);
        }
    }

    return res;
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    float scale = 0.01f;
    vector<Vec2> positions = PoissonDiskSampling2D(0.05f, 30, vec2(1.0f, 1.0f));
    std::vector<Vec4> colors(positions.size());
    for (int i = 0; i < positions.size(); i++)
    {
        //positions[i] = Vec2(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f));
        colors[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
    }


    float t = 0.0f;

    while (engine->Running())
    {
        t += 0.5f;

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        // max of positions.size() and t
        int s = std::min((int)positions.size(), (int)t);

        for(int i = 0; i < positions.size(); i++)
        {
            ShapeDrawer::Circle(positions[i], glm::vec2(scale), colors[i]);
		}

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

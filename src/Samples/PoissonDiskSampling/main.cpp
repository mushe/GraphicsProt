#include "Core/Engine.h"
#include <cmath>

using namespace std;
using namespace glm;
#include <vector>
#include <algorithm>

// original paper
// https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
vector<vec2> PoissonDiskSampling2D(float r, int k, vec2 area)
{
    vector<vec2> res;
    vector<int> activeList;
    int currentIndex = 0;

    // -----------------------------------------------
    // Step 0.
    // -----------------------------------------------
    float cellSize = r / sqrt(2.0f);
    int cellCountX = area.x / cellSize;
    int cellCountY = area.y / cellSize;
    vector<vector<int>> grid;
    for(int x = 0; x < cellCountX; x++)
    {
        vector<int> col;
        for(int y = 0; y < cellCountY; y++)
        {
            col.push_back(-1);
        }
        grid.push_back(col);
    }

    // -----------------------------------------------
    // Step 1.
    // -----------------------------------------------
    vec2 initialSample = vec2
    (
        Random::Range(0.0f, area.y), 
        Random::Range(0.0f, area.y)
    );
    int initialX = floor(initialSample.x / cellSize);
    int initialY = floor(initialSample.y / cellSize);
    grid[initialX][initialY] = currentIndex;
    activeList.push_back(currentIndex);
    res.push_back(initialSample);

    // -----------------------------------------------
    // Step 2.
    // -----------------------------------------------
    while(activeList.size() > 0)
    {
        int randomIndex = activeList[Random::Range(0, activeList.size()-1)];
        vec2 center = res[randomIndex];

        bool foundNewPoint = false;
        for(int i = 0; i < k; i++)
        {
            float theta = Random::Range(0.0f, 2.0f * PI);
            float radius = Random::Range(r, r * 2.0f);
            vec2 checkPoint = vec2
            (
                cos(theta) * radius + center.x,
                sin(theta) * radius + center.y
            );
            int ix = floor(checkPoint.x / cellSize);
            int iy = floor(checkPoint.y / cellSize);
            if(ix < 0 || iy < 0 || ix > grid.size() - 1 || iy > grid[0].size() - 1)
                continue;

            if(grid[ix][iy] == -1)
            {
                bool farFromAll = true;
                for(int j = -2; j <= 2; j++)
                {
                    for(int k = -2; k <= 2; k++)
                    {
                        int targetX = ix + j;
                        int targetY = iy + k;
                        if(targetX < 0 || targetY < 0 || targetX > grid.size() - 1 || targetY > grid[0].size() - 1)
                            continue;

                        int targetIndex = grid[targetX][targetY];
                        float dist = length(res[targetIndex] - checkPoint);
                        if(dist < r) farFromAll = false;
                    }
                }
                if(farFromAll)
                {
                    currentIndex++;
                    grid[ix][iy] = currentIndex;
                    activeList.push_back(currentIndex);
                    res.push_back(checkPoint);
                    foundNewPoint = true;
                }
            }
        }

        // remove current checking id from activelist
        if(!foundNewPoint)
        {
            for (auto it = activeList.rbegin(); it != activeList.rend(); ++it) 
            {
                if (*it == randomIndex) 
                {
                    activeList.erase(std::next(it).base());
                    break;
                }
            }
        }
    }

    return res;
}

int main()
{
    auto engine = Engine::Init(800, 800, 100, 50);

    float r = 0.05f;
    float k = 30.0f;
    
    vector<Vec2> positions = PoissonDiskSampling2D(r, k, vec2(1.0f, 1.0f));
    std::vector<Vec4> colors(positions.size());
    for (int i = 0; i < positions.size(); i++)
    {
        colors[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
    }


    float t = 0.0f;

    while (engine->Running())
    {
        t += 0.5f;

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        int s = std::min((int)positions.size(), (int)t);
        for(int i = 0; i < s; i++)
        {
            ShapeDrawer::Circle(positions[i], glm::vec2(0.01f), colors[i]);
            ShapeDrawer::Circle(positions[i], glm::vec2(r), Vec4(0.08f));
		}

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

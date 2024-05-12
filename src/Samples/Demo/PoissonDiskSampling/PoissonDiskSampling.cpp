#include "PoissonDiskSampling.h"


// poisson disk sampling
// original paper : https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
vector<Vec2> PoissonDiskSampling::PoissonDiskSampling2D(float r, int k, Vec2 area)
{
    vector<Vec2> res;
    vector<int> activeList;
    int currentIndex = 0;

    // -----------------------------------------------
    // Step 0.
    // -----------------------------------------------
    float cellSize = r / sqrt(2.0f);
    int cellCountX = area.x / cellSize;
    int cellCountY = area.y / cellSize;
    vector<vector<int>> grid;
    for (int x = 0; x < cellCountX; x++)
    {
        vector<int> col;
        for (int y = 0; y < cellCountY; y++)
        {
            col.push_back(-1);
        }
        grid.push_back(col);
    }

    // -----------------------------------------------
    // Step 1.
    // -----------------------------------------------
    Vec2 initialSample = Vec2
    (
        Random::Range(0.0f, area.y),
        Random::Range(0.0f, area.y)
    );
    int initialX = floor(initialSample.x / cellSize);
    int initialY = floor(initialSample.y / cellSize);
    initialX = min(cellCountX - 1, initialX);
    initialY = min(cellCountY - 1, initialY);
    grid[initialX][initialY] = currentIndex;
    activeList.push_back(currentIndex);
    res.push_back(initialSample);

    // -----------------------------------------------
    // Step 2.
    // -----------------------------------------------
    while (activeList.size() > 0)
    {
        int randomIndex = activeList[Random::Range(0, activeList.size() - 1)];
        Vec2 center = res[randomIndex];

        bool foundNewPoint = false;
        for (int i = 0; i < k; i++)
        {
            float theta = Random::Range(0.0f, 2.0f * PI);
            float radius = Random::Range(r, r * 2.0f);
            Vec2 checkPoint = Vec2
            (
                cos(theta) * radius + center.x,
                sin(theta) * radius + center.y
            );
            int ix = floor(checkPoint.x / cellSize);
            int iy = floor(checkPoint.y / cellSize);
            if (ix < 0 || iy < 0 || ix > grid.size() - 1 || iy > grid[0].size() - 1)
                continue;

            if (grid[ix][iy] == -1)
            {
                bool farFromAll = true;
                for (int j = -2; j <= 2; j++)
                {
                    for (int k = -2; k <= 2; k++)
                    {
                        int targetX = ix + j;
                        int targetY = iy + k;
                        if (targetX < 0 || targetY < 0 || targetX > grid.size() - 1 || targetY > grid[0].size() - 1)
                            continue;

                        int targetIndex = grid[targetX][targetY];
                        if (targetIndex == -1) continue;
                        float dist = length(res[targetIndex] - checkPoint);
                        if (dist < r) farFromAll = false;
                    }
                }
                if (farFromAll)
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
        if (!foundNewPoint)
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

void PoissonDiskSampling::Start(shared_ptr<Engine> engine)
{
    positions_ = PoissonDiskSampling2D(r_, k_, Vec2(1.0f, 1.0f));
    colors_.resize(positions_.size());
    for (int i = 0; i < positions_.size(); i++)
    {
        colors_[i] = Vec4(Random::Range(0.0f, 0.2f), Random::Range(0.1f, 0.2f), Random::Range(0.0f, 1.0f), 1.0f);
    }

}


bool PoissonDiskSampling::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    t_ += 0.5f;

    engine->BeginRenderToScreen();

    int s = std::min((int)positions_.size(), (int)t_);
    for (int i = 0; i < s; i++)
    {
        ShapeDrawer::Circle(positions_[i], glm::vec2(0.01f), colors_[i]);
        ShapeDrawer::Circle(positions_[i], glm::vec2(r_), Vec4(0.08f));
    }

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void PoissonDiskSampling::Release()
{
    positions_.clear();
    colors_.clear();
    t_ = 0.0f;
}

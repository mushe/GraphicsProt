#include "Maze.h"


void Maze::log(const vector<vector<MazeGrid>>& grids)
{
    for (int y = 0; y < grids.size(); y++)
    {
        for (int x = 0; x < grids[0].size(); x++)
        {
            std::cout << grids[y][x] << " ";
        }
        std::cout << std::endl;
    }
}

bool Maze::diggable(const vector<vector<MazeGrid>>& grids, MazePosition p)
{
    if (p.y <= 0 || p.y > grids.size() - 2) return false;
    if (p.x <= 0 || p.x > grids[0].size() - 2) return false;

    return grids[p.y][p.x] == Wall;
}

void Maze::dig(vector<vector<MazeGrid>>& grids, MazePosition p)
{
    // check random direction
    std::vector<MazePosition> directions = {
        { 0, -1 },
        { 0,  1 },
        {-1,  0 },
        { 1,  0 }
    };
    Algorithm::Shuffle(directions);

    for (int i = 0; i < 4; i++)
    {
        MazePosition checkPos = { p.x + directions[i].x * 2, p.y + directions[i].y * 2 };
        MazePosition midPos = { p.x + directions[i].x, p.y + directions[i].y };
        if (diggable(grids, checkPos))
        {
            grids[midPos.y][midPos.x] = Passage;
            grids[checkPos.y][checkPos.x] = Passage;
            dig(grids, checkPos);
        }
    }
}

void Maze::Reset()
{
    t_ = 0.0f;
    grids_.clear();

    // generate maze
    GridX_ = 41;
    GridY_ = 21;
    GridScaleX_ = 1.0f / GridX_;
    GridScaleY_ = 1.0f / GridY_;
    for (int y = 0; y < GridY_; y++)
    {
        vector<MazeGrid> row;
        for (int x = 0; x < GridX_; x++)
        {
            row.push_back((MazeGrid)0);
        }
        grids_.push_back(row);
    };

    // dig maze with hale digging algorithm
    MazePosition p = { Random::Range(1, GridX_ - 2), Random::Range(1, GridY_ - 2) };

    // convert p to odd number
    p.x = p.x % 2 == 0 ? p.x + 1 : p.x;
    p.y = p.y % 2 == 0 ? p.y + 1 : p.y;

    grids_[p.y][p.x] = Passage;
    dig(grids_, p);
    //log(grids_);
}

void Maze::Start(shared_ptr<Engine> engine)
{
    GUI::ChangeStyleToDefault();

    Reset();
}


bool Maze::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;

    t_ += 0.016f;
    if(t_ > 1.0f) Reset();


    engine->BeginRenderToScreen();

    // draw grids
    float oddScaleX = GridScaleX_ * 1.79f;
    float oddScaleY = GridScaleY_ * 1.82f;
    float evenScaleX = GridScaleX_ * 0.25f;
    float evenScaleY = GridScaleY_ * 0.25f;

    for (int y = 0; y < GridY_; y++)
    {
        for (int x = 0; x < GridX_; x++)
        {
            int grid = grids_[y][x];

            float scaleX = x % 2 == 0 ? evenScaleX : oddScaleX;
            float scaleY = y % 2 == 0 ? evenScaleY : oddScaleY;
            float posX = x / 2 * oddScaleX + (x + 1) / 2 * evenScaleX;
            float posY = y / 2 * oddScaleY + (y + 1) / 2 * evenScaleY;

            Vec2 gridPos = Vec2(posX, posY) + Vec2(scaleX, scaleY) * 0.5f;
            ShapeDrawer::Rect(gridPos, Vec2(scaleX, scaleY) * 1.0f, Vec4(grid) * 0.5f);
        }
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


void Maze::Release()
{
    grids_.clear();
}

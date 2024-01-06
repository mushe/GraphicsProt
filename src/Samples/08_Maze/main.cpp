#include "Core/Engine.h"
#include <cmath>
#include <algorithm>

enum Maze
{
    Wall,
    Passage
};

struct MazePosition
{
    int x;
    int y;
};

void log(const std::vector<std::vector<Maze>>& grids)
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

bool diggable(const std::vector<std::vector<Maze>>& grids, MazePosition p)
{
    if (p.y <= 0 || p.y > grids.size() - 2 ) return false;
    if (p.x <= 0 || p.x > grids[0].size() - 2 ) return false;

    return grids[p.y][p.x] == Wall;
}

void dig(std::vector<std::vector<Maze>>& grids, MazePosition p)
{
    //Debug::Log("Digging: " + std::to_string(p.x) + ", " + std::to_string(p.y));
    //log(grids);
    //Debug::Log("--------------------------");

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

int main()
{
    const int windowWidth = 1920;
    const int windowHeight = 1080;
    auto engine = Engine::Init(windowWidth, windowHeight, 0, 50);

    // generate maze
    int GridX = 41;
    int GridY = 21;
    float GridScaleX = 1.0f / GridX;
    float GridScaleY = 1.0f / GridY;
    std::vector<std::vector<Maze>> grids;
    for(int y = 0; y < GridY; y++)
	{
		std::vector<Maze> row;
		for(int x = 0; x < GridX; x++)
		{
			row.push_back((Maze)0);
		}
		grids.push_back(row);
    };

    // dig maze with hale digging algorithm
    MazePosition p = { Random::Range(1, GridX - 2), Random::Range(1, GridY - 2) };
    // convert p to odd number
    p.x = p.x % 2 == 0 ? p.x + 1 : p.x;
    p.y = p.y % 2 == 0 ? p.y + 1 : p.y;
    grids[p.y][p.x] = Passage;
    dig(grids, p);


    log(grids);

    while (engine->Running())
    {

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        // draw grids
        float oddScaleX = GridScaleX * 1.79f;
        float oddScaleY = GridScaleY * 1.82f;
        float evenScaleX = GridScaleX * 0.25f;
        float evenScaleY = GridScaleY * 0.25f;

        for (int y = 0; y < GridY; y++)
        {
            for (int x = 0; x < GridX; x++)
            {
                int grid = grids[y][x];

                float scaleX = x % 2 == 0 ? evenScaleX : oddScaleX;
                float scaleY = y % 2 == 0 ? evenScaleY : oddScaleY;
                float posX = x / 2 * oddScaleX + (x+1) / 2 * evenScaleX;
                float posY = y / 2 * oddScaleY + (y+1) / 2 * evenScaleY;

                Vec2 gridPos = Vec2(posX, posY) + Vec2(scaleX, scaleY) * 0.5f;
                ShapeDrawer::Rect(gridPos, Vec2(scaleX, scaleY) * 1.0f, Vec4(grid));
            }
        }

        engine->EndRenderToScreen();

        engine->OnGUI([&]()
        {
        });
        engine->EndFrame();
    }

    engine->Terminate();
}

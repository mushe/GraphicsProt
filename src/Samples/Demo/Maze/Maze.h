#pragma once
#include "Core/Scene.h"

enum MazeGrid
{
    Wall,
    Passage
};

struct MazePosition
{
    int x;
    int y;
};

class Maze : public Scene
{
public:
    Maze():
        t_(0.0f)
    {
    }
    virtual ~Maze() { }

    void Reset();

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    void log(const vector<vector<MazeGrid>>& grids);
    bool diggable(const vector<vector<MazeGrid>>& grids, MazePosition p);
    void dig(vector<vector<MazeGrid>>& grids, MazePosition p);

    // generate maze
    int GridX_;
    int GridY_;
    float GridScaleX_;
    float GridScaleY_;
    vector<vector<MazeGrid>> grids_;
    float t_;
};
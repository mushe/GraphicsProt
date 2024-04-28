#pragma once
#include "Core/Scene.h"

namespace Dungeon2DConstants
{
    enum Grid
    {
        EMPTY = 0,
        DUNGEON = 1,
        PLAYER = 2,
        GOAL = 3
    };

    const int gridNum = 50;
    const int maxDivision = 15;
    const float gridScale = 1.0f / (float)gridNum;
    const glm::uvec2 initialLeftDown = glm::uvec2(1, 1);
    const glm::uvec2 initialRightUp = glm::uvec2(gridNum - 2, gridNum - 2);
    const int minRoomSize = 11;
    const int minPadding = 1;
    const int maxPadding = 3;
}


class Dungeon2DRoom
{
private:
    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    Dungeon2DRoom* leftRoom_ = nullptr;
    Dungeon2DRoom* rightRoom_ = nullptr;
    Dungeon2DRoom* parentRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);
    int level_ = 0;
    bool verticallyDivided = false;


public:
    Dungeon2DRoom(glm::uvec2 leftDown, glm::uvec2 rightUp, Dungeon2DRoom* parentRoom, int currentLevel)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), parentRoom_(parentRoom), color_(Vec4(0.7f)), level_(currentLevel + 1), verticallyDivided(false)
    {
        if (leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y));
        }
    }

    bool connected = false;
    glm::uvec2 rightConnectionPoint = glm::uvec2(0, 0);
    glm::uvec2 leftConnectionPoint = glm::uvec2(0, 0);

    Dungeon2DRoom* GetLeft() { return leftRoom_; }
    Dungeon2DRoom* GetRight() { return rightRoom_; }
    Dungeon2DRoom* GetParent() { return parentRoom_; }
    glm::uvec2 GetLeftDown() { return leftDown_; }
    glm::uvec2 GetRightUp() { return rightUp_; }
    Vec4 GetColor() { return color_; }
    int GetLevel() { return level_; }
    bool IsVerticallyDivided() { return verticallyDivided; }

    bool IsLeaf()
    {
        return leftRoom_ == nullptr && rightRoom_ == nullptr;
    }

    bool IsDividable()
    {
        if (!IsLeaf()) return false;

        return IsDividableVertically() || IsDividableHorizontally();
    }

    bool IsDividableVertically()
    {
        return Dungeon2DConstants::minRoomSize * 2 <= (rightUp_.y - leftDown_.y + 1);
    }

    bool IsDividableHorizontally()
    {
        return Dungeon2DConstants::minRoomSize * 2 <= (rightUp_.x - leftDown_.x + 1);
    }

    void GeneratePadding()
    {
        rightUp_ = rightUp_ - glm::uvec2(Random::Range(Dungeon2DConstants::minPadding, Dungeon2DConstants::maxPadding), Random::Range(Dungeon2DConstants::minPadding, Dungeon2DConstants::maxPadding));
        leftDown_ = leftDown_ + glm::uvec2(Random::Range(Dungeon2DConstants::minPadding, Dungeon2DConstants::maxPadding), Random::Range(Dungeon2DConstants::minPadding, Dungeon2DConstants::maxPadding));
    }

    void Divide()
    {
        bool dividableVertically = IsDividableVertically();
        bool dividableHorizontally = IsDividableHorizontally();

        // not dividable both direction
        if (!dividableVertically && !dividableHorizontally) return;

        bool vertical = Random::Range(0, 1); // random direction
        if (dividableVertically && !dividableHorizontally) vertical = true; // vertical only
        if (!dividableVertically && dividableHorizontally) vertical = false; // horizontal only

        // vertical division
        if (vertical)
        {
            int divideIndex = Random::Range((int)leftDown_.y + Dungeon2DConstants::minRoomSize - 1, (int)rightUp_.y - Dungeon2DConstants::minRoomSize);
            leftRoom_ = new Dungeon2DRoom(leftDown_, glm::uvec2(rightUp_.x, divideIndex), this, level_);
            rightRoom_ = new Dungeon2DRoom(glm::uvec2(leftDown_.x, divideIndex + 1), rightUp_, this, level_);
        }
        // horizontal division
        else
        {
            int divideIndex = Random::Range((int)leftDown_.x + Dungeon2DConstants::minRoomSize - 1, (int)rightUp_.x - Dungeon2DConstants::minRoomSize);
            leftRoom_ = new Dungeon2DRoom(leftDown_, glm::uvec2(divideIndex, rightUp_.y), this, level_);
            rightRoom_ = new Dungeon2DRoom(glm::uvec2(divideIndex + 1, leftDown_.y), rightUp_, this, level_);
        }

        verticallyDivided = vertical;
    }
};




class Dungeon2D : public Scene
{
public:
    Dungeon2D():
        currentFloor_(1),
        playerPos_(glm::ivec2(0, 0)),
        playerDir_(glm::ivec2(1, 0)),
        goalPos_(glm::ivec2(0, 0))
    {
        Debug::Log("Dungeon2D()");
    }
    virtual ~Dungeon2D() { Debug::Log("~Dungeon2D()"); }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:

    vector<vector<int>> dungeon_;
    int currentFloor_;

    glm::ivec2 playerPos_;
    glm::ivec2 playerDir_;
    glm::ivec2 goalPos_;

    void GetAllRooms(Dungeon2DRoom* room, std::vector<Dungeon2DRoom*>& rooms);
    void GetAllRoomsAtLevel(Dungeon2DRoom* room, int level, std::vector<Dungeon2DRoom*>& rooms);
    void GetAllLeaves(Dungeon2DRoom* room, std::vector<Dungeon2DRoom*>& rooms);
    Dungeon2DRoom* SelectRandomRoom(Dungeon2DRoom* dungeon);
    void LogRoom(Dungeon2DRoom* room);
    void DisplayRoom(Dungeon2DRoom* room);
    void DisplayRooms(Dungeon2DRoom* rootRoom);
    int GetDeepestLevel(Dungeon2DRoom* room);
    Dungeon2DRoom* GenerateDividedRoom();
    void GeneratePaddingToRooms(Dungeon2DRoom* root);
    std::vector<std::vector<int>> ConvertRoomToVector2D(Dungeon2DRoom* root);
    void Connect(Dungeon2DRoom* room);
    void ConnectRooms(Dungeon2DRoom* root);
    void GenerateStartAndGoal(Dungeon2DRoom* room);
    Dungeon2DRoom* GenerateDungeon();
    std::vector<std::vector<int>> GenerateDungeonToData();
    void DisplayDungeonData(std::vector<std::vector<int>> dungeonData);
    void MovePlayer(std::vector<std::vector<int>>& dungeonData, glm::ivec2 direction);


};
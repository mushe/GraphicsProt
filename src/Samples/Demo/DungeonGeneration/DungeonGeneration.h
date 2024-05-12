#pragma once
#include "Core/Scene.h"

namespace DungeonGenerationConstants
{
    const int gridNum = 50;
    const int maxDivision = 15;
    const float gridScale = 1.0f / (float)gridNum;
    const glm::uvec2 initialLeftDown = glm::uvec2(1, 1);
    const glm::uvec2 initialRightUp = glm::uvec2(48, 48);
    const int minRoomSize = 11;
    const int minPadding = 1;
    const int maxPadding = 3;
}

class DungeonGenerationRoom
{
private:
    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    shared_ptr<DungeonGenerationRoom> leftRoom_ = nullptr;
    shared_ptr<DungeonGenerationRoom> rightRoom_ = nullptr; 
    DungeonGenerationRoom* parentRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);
    int level_ = 0;
    bool verticallyDivided = false;

public:
    DungeonGenerationRoom(glm::uvec2 leftDown, glm::uvec2 rightUp, DungeonGenerationRoom* parentRoom, int currentLevel)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), parentRoom_(parentRoom), color_(Vec4(0.7f)), level_(currentLevel + 1)
    {
        if (leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y));
        }
    }

    bool connected = false;
    glm::uvec2 rightConnectionPoint = glm::uvec2(0, 0);
    glm::uvec2 leftConnectionPoint = glm::uvec2(0, 0);

    shared_ptr<DungeonGenerationRoom> GetLeft() { return leftRoom_; }
    shared_ptr<DungeonGenerationRoom> GetRight() { return rightRoom_; }
    DungeonGenerationRoom* GetParent() { return parentRoom_; }
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
        return DungeonGenerationConstants::minRoomSize * 2 <= (rightUp_.y - leftDown_.y + 1);
    }

    bool IsDividableHorizontally()
    {
        return DungeonGenerationConstants::minRoomSize * 2 <= (rightUp_.x - leftDown_.x + 1);
    }

    void GeneratePadding()
    {
        rightUp_ = rightUp_ - glm::uvec2(Random::Range(DungeonGenerationConstants::minPadding, DungeonGenerationConstants::maxPadding), Random::Range(DungeonGenerationConstants::minPadding, DungeonGenerationConstants::maxPadding));
        leftDown_ = leftDown_ + glm::uvec2(Random::Range(DungeonGenerationConstants::minPadding, DungeonGenerationConstants::maxPadding), Random::Range(DungeonGenerationConstants::minPadding, DungeonGenerationConstants::maxPadding));
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
            int divideIndex = Random::Range((int)leftDown_.y + DungeonGenerationConstants::minRoomSize - 1, (int)rightUp_.y - DungeonGenerationConstants::minRoomSize);
            leftRoom_ = make_shared<DungeonGenerationRoom>(leftDown_, glm::uvec2(rightUp_.x, divideIndex), this, level_);
            rightRoom_ = make_shared<DungeonGenerationRoom>(glm::uvec2(leftDown_.x, divideIndex + 1), rightUp_, this, level_);
        }
        // horizontal division
        else
        {
            int divideIndex = Random::Range((int)leftDown_.x + DungeonGenerationConstants::minRoomSize - 1, (int)rightUp_.x - DungeonGenerationConstants::minRoomSize);
            leftRoom_ = make_shared<DungeonGenerationRoom>(leftDown_, glm::uvec2(divideIndex, rightUp_.y), this, level_);
            rightRoom_ = make_shared<DungeonGenerationRoom>(glm::uvec2(divideIndex + 1, leftDown_.y), rightUp_, this, level_);
        }

        verticallyDivided = vertical;
    }
};

class DungeonGeneration : public Scene
{
public:
    DungeonGeneration():
        t_(0.0f)
    {
    }
    virtual ~DungeonGeneration() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    float t_;
    shared_ptr<DungeonGenerationRoom> root_;

    void GetAllRooms(DungeonGenerationRoom* room, std::vector<DungeonGenerationRoom*>& rooms);
    void GetAllRoomsAtLevel(DungeonGenerationRoom* room, int level, std::vector<DungeonGenerationRoom*>& rooms);
    void GetAllLeaves(DungeonGenerationRoom* room, std::vector<DungeonGenerationRoom*>& rooms);
    void LogRoom(DungeonGenerationRoom* room);
    void DisplayRoom(DungeonGenerationRoom* room);
    void DisplayRooms(DungeonGenerationRoom* rootRoom);
    int GetDeepestLevel(DungeonGenerationRoom* room);
    shared_ptr<DungeonGenerationRoom> GenerateDividedRoom();
    void GeneratePaddingToRooms(DungeonGenerationRoom* root);

    // 0 = blank, 1 = room or corridor
    std::vector<std::vector<int>> ConvertRoomToVector2D(DungeonGenerationRoom* root);
    void Connect(DungeonGenerationRoom* room);
    void ConnectRooms(DungeonGenerationRoom* root);
    shared_ptr<DungeonGenerationRoom> GenerateDungeon();
};
#pragma once
#include "Core/Scene.h"

namespace RoomDivisionConstants
{
	const int gridNum = 50;
	const int maxDivision = 18;
	const float gridScale = 1.0f / (float)gridNum;
	const glm::uvec2 initialLeftDown = glm::uvec2(1, 1);
	const glm::uvec2 initialRightUp = glm::uvec2(48, 48);
	const int minRoomSize = 7;
}

class Room
{
private:
    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    shared_ptr<Room> leftRoom_ = nullptr;
    shared_ptr<Room> rightRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);

public:
    Room(glm::uvec2 leftDown, glm::uvec2 rightUp)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), color_(Vec4(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 0.5f))
    {
        if (leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y));
        }
    }

    shared_ptr<Room> GetLeft() { return leftRoom_; }
    shared_ptr<Room> GetRight() { return rightRoom_; }
    glm::uvec2 GetLeftDown() { return leftDown_; }
    glm::uvec2 GetRightUp() { return rightUp_; }
    Vec4 GetColor() { return color_; }

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
        return RoomDivisionConstants::minRoomSize * 2 <= (rightUp_.y - leftDown_.y + 1);
    }

    bool IsDividableHorizontally()
    {
        return RoomDivisionConstants::minRoomSize * 2 <= (rightUp_.x - leftDown_.x + 1);
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
            int divideIndex = Random::Range((int)leftDown_.y + RoomDivisionConstants::minRoomSize - 1, (int)rightUp_.y - RoomDivisionConstants::minRoomSize);
            leftRoom_ = make_shared<Room>(leftDown_, glm::uvec2(rightUp_.x, divideIndex));
            rightRoom_ = make_shared<Room>(glm::uvec2(leftDown_.x, divideIndex + 1), rightUp_);
        }
        // horizontal division
        else
        {
            int divideIndex = Random::Range((int)leftDown_.x + RoomDivisionConstants::minRoomSize - 1, (int)rightUp_.x - RoomDivisionConstants::minRoomSize);
            leftRoom_ = make_shared<Room>(leftDown_, glm::uvec2(divideIndex, rightUp_.y));
            rightRoom_ = make_shared<Room>(glm::uvec2(divideIndex + 1, leftDown_.y), rightUp_);
        }
    }
};

class RoomDivision : public Scene
{
public:
    RoomDivision():
        leftDown_(glm::vec2(0,0)),
        rightUp_(glm::vec2(0,0)),
        leftRoom_(nullptr),
        rightRoom_(nullptr),
        color_(Vec4(1,1,1,1)),
        t_(0.0f)
    {
    }
    virtual ~RoomDivision() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    glm::uvec2 leftDown_;
    glm::uvec2 rightUp_;
    shared_ptr<Room> root_;
    shared_ptr<Room> leftRoom_;
    shared_ptr<Room> rightRoom_;
    Vec4 color_;
    float t_;

    void GetAllNodes(shared_ptr<Room> room, vector<shared_ptr<Room>>& rooms);
    void GetAllLeaves(shared_ptr<Room> room, vector<shared_ptr<Room>>& rooms);
    void LogRoom(shared_ptr<Room> room);
    void DisplayRooms(shared_ptr<Room> rootRoom);
    shared_ptr<Room> GenerateDividedRoom();
};
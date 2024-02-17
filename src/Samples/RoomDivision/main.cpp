#include "Core/Engine.h"
#include <cmath>

const int gridNum = 50;
const int maxDivision = 18;
const float gridScale = 1.0f / (float) gridNum;
const glm::uvec2 initialLeftDown = glm::uvec2(1, 1);
const glm::uvec2 initialRightUp = glm::uvec2(48, 48);
const int minRoomSize = 7;

class Room
{
private:
    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    Room* leftRoom_ = nullptr;
    Room* rightRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);

public:
    Room(glm::uvec2 leftDown, glm::uvec2 rightUp)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), color_(Vec4(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 1.0f))
    {
        if(leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y) );
        }
    }

    Room* GetLeft(){return leftRoom_;}
    Room* GetRight(){return rightRoom_;}
    glm::uvec2 GetLeftDown(){return leftDown_;}
    glm::uvec2 GetRightUp(){return rightUp_;}
    Vec4 GetColor(){return color_;}

    bool IsLeaf()
    {
        return leftRoom_ == nullptr && rightRoom_ == nullptr;
    }

    bool IsDividable()
    {
        if(!IsLeaf()) return false;
        
        return IsDividableVertically() || IsDividableHorizontally();
    }

    bool IsDividableVertically()
    {
        return minRoomSize * 2 <= (rightUp_.y - leftDown_.y + 1);
    }

    bool IsDividableHorizontally()
    {
        return minRoomSize * 2 <= (rightUp_.x - leftDown_.x + 1);
    }

    void Divide()
    {
        bool dividableVertically = IsDividableVertically();
        bool dividableHorizontally = IsDividableHorizontally();

        // not dividable both direction
        if(!dividableVertically && !dividableHorizontally) return;

        bool vertical = Random::Range(0, 1); // random direction
        if(dividableVertically && !dividableHorizontally) vertical = true; // vertical only
        if(!dividableVertically && dividableHorizontally) vertical = false; // horizontal only

        // vertical division
        if(vertical)
        {
            int divideIndex = Random::Range((int)leftDown_.y + minRoomSize - 1, (int)rightUp_.y - minRoomSize);
            leftRoom_ = new Room(leftDown_, glm::uvec2(rightUp_.x, divideIndex));
            rightRoom_ = new Room(glm::uvec2(leftDown_.x, divideIndex + 1), rightUp_);
        }
        // horizontal division
        else
        {
            int divideIndex = Random::Range((int)leftDown_.x + minRoomSize - 1, (int)rightUp_.x - minRoomSize);
            leftRoom_ = new Room(leftDown_, glm::uvec2(divideIndex, rightUp_.y));
            rightRoom_ = new Room(glm::uvec2(divideIndex + 1, leftDown_.y), rightUp_);
            
        }
    }
};

void GetAllNodes(Room* room, std::vector<Room*>& rooms)
{
    if(room == nullptr) return;

    rooms.push_back(room);

    if(!room->IsLeaf())
    {
        if(room->GetLeft() != nullptr) GetAllNodes(room->GetLeft(), rooms);
        if(room->GetRight() != nullptr) GetAllNodes(room->GetRight(), rooms);
    }
}

void GetAllLeaves(Room* room, std::vector<Room*>& rooms)
{
    if(room == nullptr) return;

    if(room->IsLeaf())
    {
        rooms.push_back(room);
    }
    else
    {
        if(room->GetLeft() != nullptr) GetAllLeaves(room->GetLeft(), rooms);
        if(room->GetRight() != nullptr) GetAllLeaves(room->GetRight(), rooms);
    }
}

void LogRoom(Room* room)
{
    if(room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->GetLeftDown().x) + ", " + to_string(room->GetLeftDown().y) + " rightUp: " + to_string(room->GetRightUp().x) + ", " + to_string(room->GetRightUp().y));

}

void DisplayRooms(Room* rootRoom)
{
    std::vector<Room*> rooms;
    GetAllLeaves(rootRoom, rooms);

    //Debug::Log("----------------------");
    for(auto room : rooms)
    {
        //LogRoom(room);
        float x = (room->GetRightUp().x + room->GetLeftDown().x) * 0.5f / (float)gridNum + gridScale * 0.5;
        float y = 1.0f - (room->GetRightUp().y + room->GetLeftDown().y) * 0.5f / (float)gridNum - gridScale * 0.5;
        float xScale = ((room->GetRightUp().x - room->GetLeftDown().x) + 1.0f) / (float)gridNum;
        float yScale = ((room->GetRightUp().y - room->GetLeftDown().y) + 1.0f) / (float)gridNum;
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), room->GetColor());
    }
}


Room* GenerateDividedRoom()
{
    Room* root = new Room(initialLeftDown, initialRightUp);
    root->Divide();
    for(int i = 0; i < maxDivision; i++)
    {
        auto leaves = std::vector<Room*>();
        GetAllLeaves(root, leaves);

        auto dividableLeaves = std::vector<Room*>();
        for(auto leaf : leaves)
        {
            if(leaf->IsDividable()) dividableLeaves.push_back(leaf);
        }

        if(dividableLeaves.size() == 0) break;

        dividableLeaves[Random::Range(0, dividableLeaves.size()-1)]->Divide();
    }

    return root;
}



float t = 0.0f;

int main()
{
    auto engine = Engine::Init();

    Room* root = GenerateDividedRoom();

    while (engine->Running())
    {

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        t += 0.016f;
        if (Input::KeyDown(KeyCode::R) || t > 1.0f)
        {
            //TODO cleanup room before regenerate
            root = GenerateDividedRoom();

            t = 0.0f;
        }

        DisplayRooms(root);

        // display grids
        for (int i = 0; i < gridNum; i++)
        {
            for (int j = 0; j < gridNum; j++)
            {
                float x = i * gridScale;
                float y = j * gridScale;
                ShapeDrawer::Rect(Vec2(x, y) + gridScale * 0.5f, Vec2(gridScale * 0.9), Vec4(0.1f, 0.1f, 0.1f, 1.0f));
            }
        }

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

#include "Core/Engine.h"
#include <cmath>


const int GridNum = 50;
const float GridScale = 1.0f / (float) GridNum;

class Room
{
private:
    const int minRoomSize = 5;
public:
    Room(glm::uvec2 leftDown, glm::uvec2 rightUp)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), color_(Vec4(Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 1.0f))
    {
        if(leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y) );
        }
    }

    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    Room* leftRoom_ = nullptr;
    Room* rightRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);


    bool IsLeaf()
    {
        return leftRoom_ == nullptr && rightRoom_ == nullptr;
    }

    bool IsDividable()
    {
        if(!IsLeaf()) return false;
        
        return (rightUp_.x - leftDown_.x > minRoomSize * 2) || (rightUp_.y - leftDown_.y > minRoomSize * 2);
    }

    void Divide()
    {
        if(!IsDividable()) return;
        bool vertical = Random::Range(0, 1);
        // vertical division
        if(vertical)
        {
            int divideX = Random::Range((int)leftDown_.x + minRoomSize, (int)rightUp_.x - minRoomSize);
            leftRoom_ = new Room(leftDown_, glm::uvec2(divideX, rightUp_.y));
            rightRoom_ = new Room(glm::uvec2(divideX + 1, leftDown_.y), rightUp_);
        }
        // horizontal division
        else
        {
            int divideY = Random::Range((int)leftDown_.y + minRoomSize, (int)rightUp_.y - minRoomSize);
            leftRoom_ = new Room(leftDown_, glm::uvec2(rightUp_.x, divideY));
            rightRoom_ = new Room(glm::uvec2(leftDown_.x, divideY  + 1), rightUp_);
        }
    }
};

void GetAllNodes(Room* room, std::vector<Room*>& rooms)
{
    if(room == nullptr) return;

    rooms.push_back(room);

    if(!room->IsLeaf())
    {
        if(room->leftRoom_ != nullptr) GetAllNodes(room->leftRoom_, rooms);
        if(room->rightRoom_ != nullptr) GetAllNodes(room->rightRoom_, rooms);
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
        if(room->leftRoom_ != nullptr) GetAllLeaves(room->leftRoom_, rooms);
        if(room->rightRoom_ != nullptr) GetAllLeaves(room->rightRoom_, rooms);
    }
}

void LogRoom(Room* room)
{
    if(room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->leftDown_.x) + ", " + to_string(room->leftDown_.y) + " rightUp: " + to_string(room->rightUp_.x) + ", " + to_string(room->rightUp_.y));

}

void DisplayRooms(Room* rootRoom)
{
    std::vector<Room*> rooms;
    GetAllLeaves(rootRoom, rooms);

    //Debug::Log("----------------------");
    for(auto room : rooms)
    {
        //LogRoom(room);
        float x = (room->rightUp_.x + room->leftDown_.x) * 0.5f / (float)GridNum + GridScale * 0.5;
        float y = 1.0f - (room->rightUp_.y + room->leftDown_.y) * 0.5f / (float)GridNum - GridScale * 0.5;
        float xScale = ((room->rightUp_.x - room->leftDown_.x) + 1.0f) / (float)GridNum;
        float yScale = ((room->rightUp_.y - room->leftDown_.y) + 1.0f) / (float)GridNum;
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), room->color_);
    }
}




int main()
{
    auto engine = Engine::Init();

    Room* root = new Room(glm::uvec2(1, 1), glm::uvec2(GridNum-2, GridNum-2));
    root->Divide();

    // for(int i = 0; i < 10; i++)
    // {
    //     auto leaves = std::vector<Room*>();
    //     GetAllLeaves(root, leaves);

    //     auto dividableLeaves = std::vector<Room*>();
    //     for(auto leaf : leaves)
    //     {
    //         if(leaf->IsDividable()) dividableLeaves.push_back(leaf);
    //     }

    //     if(dividableLeaves.size() == 0) break;

    //     dividableLeaves[Random::Range(0, dividableLeaves.size()-1)]->Divide();
    // }


    while (engine->Running())
    {

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        DisplayRooms(root);

        // display grids
        for (int i = 0; i < GridNum; i++)
        {
            for (int j = 0; j < GridNum; j++)
            {
                float x = i * GridScale;
                float y = j * GridScale;
                ShapeDrawer::Rect(Vec2(x, y) + GridScale * 0.5f, Vec2(GridScale * 0.9), Vec4(0.1f, 0.1f, 0.1f, 1.0f));
            }
        }

        //ShapeDrawer::Rect(positions[i], Vec2(GridScale * 0.9), colors[i]);

        // engine->OnGUI([&]()
        // {
        // });

        engine->EndRenderToScreen();
        engine->EndFrame();
    }

    engine->Terminate();
}

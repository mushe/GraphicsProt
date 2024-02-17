#include "Core/Engine.h"
#include <cmath>

const int gridNum = 50;
const int maxDivision = 15;
const float gridScale = 1.0f / (float) gridNum;
const glm::uvec2 initialLeftDown = glm::uvec2(1, 1);
const glm::uvec2 initialRightUp = glm::uvec2(48, 48);
const int minRoomSize = 11;
const int minPadding = 1;
const int maxPadding = 3;

class Room
{
private:
    glm::uvec2 leftDown_ = glm::uvec2(0, 0);
    glm::uvec2 rightUp_ = glm::uvec2(0, 0);
    Room* leftRoom_ = nullptr;
    Room* rightRoom_ = nullptr;
    Room* parentRoom_ = nullptr;
    Vec4 color_ = Vec4(1, 1, 1, 1);
    int level_ = 0;
    bool verticallyDivided = false;

public:
    Room(glm::uvec2 leftDown, glm::uvec2 rightUp, Room* parentRoom, int currentLevel)
        : leftDown_(leftDown), rightUp_(rightUp), leftRoom_(nullptr), rightRoom_(nullptr), parentRoom_(parentRoom), color_(Vec4(0.7f)), level_(currentLevel + 1)
    {
        if(leftDown_.x > rightUp_.x || leftDown_.y > rightUp_.y)
        {
            Debug::Log("invalid room.  rightUp:" + to_string(rightUp_.x) + ", " + to_string(rightUp_.y) + " leftDown:" + to_string(leftDown_.x) + ", " + to_string(leftDown_.y) );
        }
    }

    bool connected = false;
    glm::uvec2 rightConnectionPoint = glm::uvec2(0,0);
    glm::uvec2 leftConnectionPoint = glm::uvec2(0,0);

    Room* GetLeft(){return leftRoom_;}
    Room* GetRight(){return rightRoom_;}
    Room* GetParent(){return parentRoom_;}
    glm::uvec2 GetLeftDown(){return leftDown_;}
    glm::uvec2 GetRightUp(){return rightUp_;}
    Vec4 GetColor(){return color_;}
    int GetLevel(){return level_;}
    bool IsVerticallyDivided(){return verticallyDivided;}

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

    void GeneratePadding()
    {
        rightUp_ = rightUp_ - glm::uvec2(Random::Range(minPadding, maxPadding), Random::Range(minPadding, maxPadding));
        leftDown_ = leftDown_ + glm::uvec2(Random::Range(minPadding, maxPadding), Random::Range(minPadding, maxPadding));
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
            leftRoom_ = new Room(leftDown_, glm::uvec2(rightUp_.x, divideIndex), this,level_);
            rightRoom_ = new Room(glm::uvec2(leftDown_.x, divideIndex + 1),rightUp_, this, level_);
        }
        // horizontal division
        else
        {
            int divideIndex = Random::Range((int)leftDown_.x + minRoomSize - 1, (int)rightUp_.x - minRoomSize);
            leftRoom_ = new Room(leftDown_, glm::uvec2(divideIndex, rightUp_.y), this, level_);
            rightRoom_ = new Room(glm::uvec2(divideIndex + 1, leftDown_.y), rightUp_, this, level_);
        }

        verticallyDivided = vertical;
    }
};

void GetAllRooms(Room* room, std::vector<Room*>& rooms)
{
    if(room == nullptr) return;

    rooms.push_back(room);

    if(!room->IsLeaf())
    {
        if(room->GetLeft() != nullptr) GetAllRooms(room->GetLeft(), rooms);
        if(room->GetRight() != nullptr) GetAllRooms(room->GetRight(), rooms);
    }
}

void GetAllRoomsAtLevel(Room* room, int level, std::vector<Room*>& rooms)
{
    if(room == nullptr) return;

    if(room->GetLevel() == level)
    {
        rooms.push_back(room);
    }

    if(!room->IsLeaf())
    {
        if(room->GetLeft() != nullptr) GetAllRoomsAtLevel(room->GetLeft(), level, rooms);
        if(room->GetRight() != nullptr) GetAllRoomsAtLevel(room->GetRight(), level, rooms);
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

void DisplayRoom(Room* room)
{
    float x = (room->GetRightUp().x + room->GetLeftDown().x) * 0.5f / (float)gridNum + gridScale * 0.5;
    float y = 1.0f - (room->GetRightUp().y + room->GetLeftDown().y) * 0.5f / (float)gridNum - gridScale * 0.5;
    float xScale = ((room->GetRightUp().x - room->GetLeftDown().x) + 1.0f) / (float)gridNum;
    float yScale = ((room->GetRightUp().y - room->GetLeftDown().y) + 1.0f) / (float)gridNum;

    // display room itself
    if(room->IsLeaf())
    {
        //Text(to_string(room->GetLevel()), Vec2(x,y), 1.0f, Vec4(0,0,0,1));
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), room->GetColor());
    }

    // display connection (corridor)
    if(room->GetParent() == nullptr) return;
    auto parent = room->GetParent();
    if(parent->connected)
    {
        ShapeDrawer::Rect
        (
            Vec2
            (
                (float)(parent->leftConnectionPoint.x + parent->rightConnectionPoint.x) * 0.5f / (float)gridNum + gridScale * 0.5f,
                1.0f - (float)(parent->leftConnectionPoint.y + parent->rightConnectionPoint.y) * 0.5f / (float)gridNum - gridScale * 0.5f
            ), 
            Vec2
            (
                (float)(parent->rightConnectionPoint.x - parent->leftConnectionPoint.x) / (float)gridNum + gridScale,
                (float)(parent->rightConnectionPoint.y - parent->leftConnectionPoint.y) / (float)gridNum + gridScale
            ), 
            Vec4(0.7f)
        );
    }
}

void DisplayRooms(Room* rootRoom)
{
    std::vector<Room*> leaves;
    GetAllRooms(rootRoom, leaves);
    //GetAllLeaves(rootRoom, leaves);

    for(auto leaf : leaves)
    {
        //LogRoom(room);
        DisplayRoom(leaf);

        if(leaf->GetParent() != nullptr)
        {
            auto right = leaf->GetParent()->GetRight();
            auto left = leaf->GetParent()->GetLeft();
        }
    }
}

int GetDeepestLevel(Room* room)
{
    if(room == nullptr) return 0;

    auto leaves = std::vector<Room*>();
    GetAllLeaves(room, leaves);

    int deepestLevel = 0;
    for(auto leaf : leaves)
    {
        int l = leaf->GetLevel();
        if(l > deepestLevel) deepestLevel = l;
    }
    return deepestLevel;
}


Room* GenerateDividedRoom()
{
    Room* root = new Room(initialLeftDown, initialRightUp, nullptr,0);
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

void GeneratePaddingToRooms(Room* root)
{
    std::vector<Room*> leaves;
    GetAllLeaves(root, leaves);

    for(auto leaf : leaves)
    {
        leaf->GeneratePadding();
    }
}

// 0 = blank, 1 = room or corridor
std::vector<std::vector<int>> ConvertRoomToVector2D(Room* root)
{
    std::vector<std::vector<int>> result = std::vector<std::vector<int>>(gridNum, std::vector<int>(gridNum, 0));
    
    std::vector<Room*> rooms;
    GetAllRooms(root, rooms);


    for(auto room : rooms)
    {
        // not leaf room(connected)
        if(room->connected)
        {
            for(int i = room->leftConnectionPoint.x; i <= room->rightConnectionPoint.x; i++)
            {
                for(int j = room->leftConnectionPoint.y; j <= room->rightConnectionPoint.y; j++)
                {
                    result[j][i] = 1;
                }
            }
        }
        // leaf room
        else
        {
            for(int i = room->GetLeftDown().x; i <= room->GetRightUp().x; i++)
            {
                for(int j = room->GetLeftDown().y; j <= room->GetRightUp().y; j++)
                {
                    result[j][i] = 1;
                }
            }
        }
        
    }

    return result;
}

// connect left and right room
void Connect(Room* room)
{
    // connect vertically
    if(room->IsVerticallyDivided())
    {
        int connectX = Random::Range
        (
            (int)room->GetLeftDown().x + maxPadding,
            (int)room->GetRightUp().x - maxPadding
        );
        auto up = ConvertRoomToVector2D(room->GetRight());
        auto down = ConvertRoomToVector2D(room->GetLeft());

        int connectYup = 0;
        int connectYdown = 0;
        for(int i = 0; i < gridNum; i++)
        {
            if(up[i][connectX] == 1)
            {
                connectYup = i - 1;
                break;
            }
        }

        for(int i = gridNum - 1; i >= 0; i--)
        {
            if(down[i][connectX] == 1)
            {
                connectYdown = i;
                break;
            }
        }

        room->leftConnectionPoint = glm::uvec2(connectX, connectYdown);
        room->rightConnectionPoint = glm::uvec2(connectX, connectYup);

    }
    // conenct horizontally
    else
    {
        int connectY = Random::Range
        (
            (int)room->GetLeftDown().y + maxPadding,
            (int)room->GetRightUp().y - maxPadding
        );

        auto left = ConvertRoomToVector2D(room->GetLeft());
        auto right = ConvertRoomToVector2D(room->GetRight());


        int connectXleft = 0;
        int connectXright = 0;
        for(int i = 0; i < gridNum; i++)
        {
            if(right[connectY][i] == 1)
            {
                connectXright = i - 1;
                break;
            }
        }

        for(int i = gridNum - 1; i >= 0; i--)
        {
            if(left[connectY][i] == 1)
            {
                connectXleft = i;
                break;
            }
        }

        room->leftConnectionPoint = glm::uvec2(connectXleft, connectY);
        room->rightConnectionPoint = glm::uvec2(connectXright, connectY);
    }

    room->connected = true;
}

void ConnectRooms(Room* root)
{
    int deepestLevel = GetDeepestLevel(root);

    std::vector<Room*> rooms;
    
    // from deepest level to 2
    for(int i = deepestLevel; i > 1; i--)
    {
        int level = i;
        GetAllRoomsAtLevel(root, level, rooms);

        for(int i = 0; i < rooms.size(); i++)
        {
            if(!rooms[i]->GetParent()->connected) 
                Connect(rooms[i]->GetParent());
        }
    }
}


Room* GenerateDungeon()
{
    // divide rooms
    Room* root = GenerateDividedRoom();

    // add padding
    GeneratePaddingToRooms(root);

    // connect rooms
    ConnectRooms(root);

    return root;
}



int main()
{
    auto engine = Engine::Init();

    auto root = GenerateDungeon();

    while (engine->Running())
    {

        engine->BeginFrame();
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        engine->BeginRenderToScreen();

        if (Input::KeyDown(KeyCode::R))
        {
            //TODO cleanup room before regenerate
            root = GenerateDungeon();
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

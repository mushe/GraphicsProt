#include "DungeonGeneration.h"

using namespace DungeonGenerationConstants;

void DungeonGeneration::GetAllRooms(DungeonGenerationRoom* room, std::vector<DungeonGenerationRoom*>& rooms)
{
    if (room == nullptr) return;

    rooms.push_back(room);

    if (!room->IsLeaf())
    {
        if (room->GetLeft() != nullptr) GetAllRooms(room->GetLeft(), rooms);
        if (room->GetRight() != nullptr) GetAllRooms(room->GetRight(), rooms);
    }
}

void DungeonGeneration::GetAllRoomsAtLevel(DungeonGenerationRoom* room, int level, std::vector<DungeonGenerationRoom*>& rooms)
{
    if (room == nullptr) return;

    if (room->GetLevel() == level)
    {
        rooms.push_back(room);
    }

    if (!room->IsLeaf())
    {
        if (room->GetLeft() != nullptr) GetAllRoomsAtLevel(room->GetLeft(), level, rooms);
        if (room->GetRight() != nullptr) GetAllRoomsAtLevel(room->GetRight(), level, rooms);
    }
}

void DungeonGeneration::GetAllLeaves(DungeonGenerationRoom* room, std::vector<DungeonGenerationRoom*>& rooms)
{
    if (room == nullptr) return;

    if (room->IsLeaf())
    {
        rooms.push_back(room);
    }
    else
    {
        if (room->GetLeft() != nullptr) GetAllLeaves(room->GetLeft(), rooms);
        if (room->GetRight() != nullptr) GetAllLeaves(room->GetRight(), rooms);
    }
}

void DungeonGeneration::LogRoom(DungeonGenerationRoom* room)
{
    if (room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->GetLeftDown().x) + ", " + to_string(room->GetLeftDown().y) + " rightUp: " + to_string(room->GetRightUp().x) + ", " + to_string(room->GetRightUp().y));

}

void DungeonGeneration::DisplayRoom(DungeonGenerationRoom* room)
{
    float x = (room->GetRightUp().x + room->GetLeftDown().x) * 0.5f / (float)gridNum + gridScale * 0.5;
    float y = 1.0f - (room->GetRightUp().y + room->GetLeftDown().y) * 0.5f / (float)gridNum - gridScale * 0.5;
    float xScale = ((room->GetRightUp().x - room->GetLeftDown().x) + 1.0f) / (float)gridNum;
    float yScale = ((room->GetRightUp().y - room->GetLeftDown().y) + 1.0f) / (float)gridNum;

    // display room itself
    if (room->IsLeaf())
    {
        //Text(to_string(room->GetLevel()), Vec2(x,y), 1.0f, Vec4(0,0,0,1));
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), Vec4(0.5f));
    }

    // display connection (corridor)
    if (room->GetParent() == nullptr) return;
    auto parent = room->GetParent();
    if (parent->connected)
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
            Vec4(0.5f)
        );
    }
}

void DungeonGeneration::DisplayRooms(DungeonGenerationRoom* rootRoom)
{
    std::vector<DungeonGenerationRoom*> leaves;
    GetAllRooms(rootRoom, leaves);
    //GetAllLeaves(rootRoom, leaves);

    for (auto leaf : leaves)
    {
        //LogRoom(room);
        DisplayRoom(leaf);

        if (leaf->GetParent() != nullptr)
        {
            auto right = leaf->GetParent()->GetRight();
            auto left = leaf->GetParent()->GetLeft();
        }
    }
}

int DungeonGeneration::GetDeepestLevel(DungeonGenerationRoom* room)
{
    if (room == nullptr) return 0;

    auto leaves = std::vector<DungeonGenerationRoom*>();
    GetAllLeaves(room, leaves);

    int deepestLevel = 0;
    for (auto leaf : leaves)
    {
        int l = leaf->GetLevel();
        if (l > deepestLevel) deepestLevel = l;
    }
    return deepestLevel;
}


DungeonGenerationRoom* DungeonGeneration::GenerateDividedRoom()
{
    DungeonGenerationRoom* root = new DungeonGenerationRoom(initialLeftDown, initialRightUp, nullptr, 0);
    root->Divide();
    for (int i = 0; i < maxDivision; i++)
    {
        auto leaves = std::vector<DungeonGenerationRoom*>();
        GetAllLeaves(root, leaves);

        auto dividableLeaves = std::vector<DungeonGenerationRoom*>();
        for (auto leaf : leaves)
        {
            if (leaf->IsDividable()) dividableLeaves.push_back(leaf);
        }

        if (dividableLeaves.size() == 0) break;

        dividableLeaves[Random::Range(0, dividableLeaves.size() - 1)]->Divide();
    }

    return root;
}

void DungeonGeneration::GeneratePaddingToRooms(DungeonGenerationRoom* root)
{
    std::vector<DungeonGenerationRoom*> leaves;
    GetAllLeaves(root, leaves);

    for (auto leaf : leaves)
    {
        leaf->GeneratePadding();
    }
}

// 0 = blank, 1 = room or corridor
std::vector<std::vector<int>> DungeonGeneration::ConvertRoomToVector2D(DungeonGenerationRoom* root)
{
    std::vector<std::vector<int>> result = std::vector<std::vector<int>>(gridNum, std::vector<int>(gridNum, 0));

    std::vector<DungeonGenerationRoom*> rooms;
    GetAllRooms(root, rooms);


    for (auto room : rooms)
    {
        // not leaf room(connected)
        if (room->connected)
        {
            for (int i = room->leftConnectionPoint.x; i <= room->rightConnectionPoint.x; i++)
            {
                for (int j = room->leftConnectionPoint.y; j <= room->rightConnectionPoint.y; j++)
                {
                    result[j][i] = 1;
                }
            }
        }
        // leaf room
        else
        {
            for (int i = room->GetLeftDown().x; i <= room->GetRightUp().x; i++)
            {
                for (int j = room->GetLeftDown().y; j <= room->GetRightUp().y; j++)
                {
                    result[j][i] = 1;
                }
            }
        }

    }

    return result;
}

// connect left and right room
void DungeonGeneration::Connect(DungeonGenerationRoom* room)
{
    // connect vertically
    if (room->IsVerticallyDivided())
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
        for (int i = 0; i < gridNum; i++)
        {
            if (up[i][connectX] == 1)
            {
                connectYup = i - 1;
                break;
            }
        }

        for (int i = gridNum - 1; i >= 0; i--)
        {
            if (down[i][connectX] == 1)
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
        for (int i = 0; i < gridNum; i++)
        {
            if (right[connectY][i] == 1)
            {
                connectXright = i - 1;
                break;
            }
        }

        for (int i = gridNum - 1; i >= 0; i--)
        {
            if (left[connectY][i] == 1)
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

void DungeonGeneration::ConnectRooms(DungeonGenerationRoom* root)
{
    int deepestLevel = GetDeepestLevel(root);

    std::vector<DungeonGenerationRoom*> rooms;

    // from deepest level to 2
    for (int i = deepestLevel; i > 1; i--)
    {
        int level = i;
        GetAllRoomsAtLevel(root, level, rooms);

        for (int i = 0; i < rooms.size(); i++)
        {
            if (!rooms[i]->GetParent()->connected)
                Connect(rooms[i]->GetParent());
        }
    }
}


DungeonGenerationRoom* DungeonGeneration::GenerateDungeon()
{
    // divide rooms
    DungeonGenerationRoom* root = GenerateDividedRoom();

    // add padding
    GeneratePaddingToRooms(root);

    // connect rooms
    ConnectRooms(root);

    return root;
}

void DungeonGeneration::Start(shared_ptr<Engine> engine)
{
    Debug::Log("DungeonGeneration::Start()");
    root_ = GenerateDungeon();
}


bool DungeonGeneration::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    t_ += 0.016f;
    if (Input::KeyDown(InputCode::Key::R) || t_ > 1.0f)
    {
        //TODO cleanup room before regenerate
        root_ = GenerateDungeon();

        t_ = 0.0f;
    }

    DisplayRooms(root_);

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

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void DungeonGeneration::Release()
{
    Debug::Log("DungeonGeneration::Release()");
}

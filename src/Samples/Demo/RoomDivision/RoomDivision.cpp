#include "RoomDivision.h"

using namespace RoomDivisionConstants;


void RoomDivision::GetAllNodes(Room* room, std::vector<Room*>& rooms)
{
    if (room == nullptr) return;

    rooms.push_back(room);

    if (!room->IsLeaf())
    {
        if (room->GetLeft() != nullptr) GetAllNodes(room->GetLeft(), rooms);
        if (room->GetRight() != nullptr) GetAllNodes(room->GetRight(), rooms);
    }
}

void RoomDivision::GetAllLeaves(Room* room, std::vector<Room*>& rooms)
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

void RoomDivision::LogRoom(Room* room)
{
    if (room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->GetLeftDown().x) + ", " + to_string(room->GetLeftDown().y) + " rightUp: " + to_string(room->GetRightUp().x) + ", " + to_string(room->GetRightUp().y));

}

void RoomDivision::DisplayRooms(Room* rootRoom)
{
    std::vector<Room*> rooms;
    GetAllLeaves(rootRoom, rooms);

    //Debug::Log("----------------------");
    for (auto room : rooms)
    {
        //LogRoom(room);
        float x = (room->GetRightUp().x + room->GetLeftDown().x) * 0.5f / (float)gridNum + gridScale * 0.5;
        float y = 1.0f - (room->GetRightUp().y + room->GetLeftDown().y) * 0.5f / (float)gridNum - gridScale * 0.5;
        float xScale = ((room->GetRightUp().x - room->GetLeftDown().x) + 1.0f) / (float)gridNum;
        float yScale = ((room->GetRightUp().y - room->GetLeftDown().y) + 1.0f) / (float)gridNum;
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), room->GetColor());
    }
}


Room* RoomDivision::GenerateDividedRoom()
{
    Room* root = new Room(initialLeftDown, initialRightUp);
    root->Divide();
    for (int i = 0; i < maxDivision; i++)
    {
        auto leaves = std::vector<Room*>();
        GetAllLeaves(root, leaves);

        auto dividableLeaves = std::vector<Room*>();
        for (auto leaf : leaves)
        {
            if (leaf->IsDividable()) dividableLeaves.push_back(leaf);
        }

        if (dividableLeaves.size() == 0) break;

        dividableLeaves[Random::Range(0, dividableLeaves.size() - 1)]->Divide();
    }

    return root;
}


void RoomDivision::Start(shared_ptr<Engine> engine)
{
    root_ = GenerateDividedRoom();
}


bool RoomDivision::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    t_ += 0.016f;
    if (Input::KeyDown(InputCode::Key::R) || t_ > 1.0f)
    {
        //TODO cleanup room before regenerate
        root_ = GenerateDividedRoom();

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


void RoomDivision::Release()
{
}

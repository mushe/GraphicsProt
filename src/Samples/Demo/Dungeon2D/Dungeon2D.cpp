#include "Dungeon2D.h"

using namespace Dungeon2DConstants;

void Dungeon2D::GetAllRooms(Dungeon2DRoom* room, std::vector<Dungeon2DRoom*>& rooms)
{
    if (room == nullptr) return;

    rooms.push_back(room);

    if (!room->IsLeaf())
    {
        if (room->GetLeft() != nullptr) GetAllRooms(room->GetLeft(), rooms);
        if (room->GetRight() != nullptr) GetAllRooms(room->GetRight(), rooms);
    }
}

void Dungeon2D::GetAllRoomsAtLevel(Dungeon2DRoom* room, int level, std::vector<Dungeon2DRoom*>& rooms)
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

void Dungeon2D::GetAllLeaves(Dungeon2DRoom* room, std::vector<Dungeon2DRoom*>& rooms)
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

Dungeon2DRoom* Dungeon2D::SelectRandomRoom(Dungeon2DRoom* dungeon)
{
    std::vector<Dungeon2DRoom*> rooms;
    GetAllLeaves(dungeon, rooms);
    return rooms[Random::Range(0, rooms.size() - 1)];
}

void Dungeon2D::LogRoom(Dungeon2DRoom* room)
{
    if (room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->GetLeftDown().x) + ", " + to_string(room->GetLeftDown().y) + " rightUp: " + to_string(room->GetRightUp().x) + ", " + to_string(room->GetRightUp().y));

}

void Dungeon2D::DisplayRoom(Dungeon2DRoom* room)
{
    float x = (room->GetRightUp().x + room->GetLeftDown().x) * 0.5f / (float)gridNum + gridScale * 0.5;
    float y = 1.0f - (room->GetRightUp().y + room->GetLeftDown().y) * 0.5f / (float)gridNum - gridScale * 0.5;
    float xScale = ((room->GetRightUp().x - room->GetLeftDown().x) + 1.0f) / (float)gridNum;
    float yScale = ((room->GetRightUp().y - room->GetLeftDown().y) + 1.0f) / (float)gridNum;

    // display room itself
    if (room->IsLeaf())
    {
        //Text(to_string(room->GetLevel()), Vec2(x,y), 1.0f, Vec4(0,0,0,1));
        ShapeDrawer::Rect(Vec2(x, y), Vec2(xScale, yScale), room->GetColor());
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
            Vec4(0.7f)
        );
    }
}

void Dungeon2D::DisplayRooms(Dungeon2DRoom* rootRoom)
{
    std::vector<Dungeon2DRoom*> leaves;
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

int Dungeon2D::GetDeepestLevel(Dungeon2DRoom* room)
{
    if (room == nullptr) return 0;

    auto leaves = std::vector<Dungeon2DRoom*>();
    GetAllLeaves(room, leaves);

    int deepestLevel = 0;
    for (auto leaf : leaves)
    {
        int l = leaf->GetLevel();
        if (l > deepestLevel) deepestLevel = l;
    }
    return deepestLevel;
}

Dungeon2DRoom* Dungeon2D::GenerateDividedRoom()
{
    Dungeon2DRoom* root = new Dungeon2DRoom(initialLeftDown, initialRightUp, nullptr, 0);
    root->Divide();
    for (int i = 0; i < maxDivision; i++)
    {
        auto leaves = std::vector<Dungeon2DRoom*>();
        GetAllLeaves(root, leaves);

        auto dividableLeaves = std::vector<Dungeon2DRoom*>();
        for (auto leaf : leaves)
        {
            if (leaf->IsDividable()) dividableLeaves.push_back(leaf);
        }

        if (dividableLeaves.size() == 0) break;

        dividableLeaves[Random::Range(0, dividableLeaves.size() - 1)]->Divide();
    }

    return root;
}

void Dungeon2D::GeneratePaddingToRooms(Dungeon2DRoom* root)
{
    std::vector<Dungeon2DRoom*> leaves;
    GetAllLeaves(root, leaves);

    for (auto leaf : leaves)
    {
        leaf->GeneratePadding();
    }
}

std::vector<std::vector<int>> Dungeon2D::ConvertRoomToVector2D(Dungeon2DRoom* root)
{
    std::vector<std::vector<int>> result = std::vector<std::vector<int>>(gridNum, std::vector<int>(gridNum, 0));

    std::vector<Dungeon2DRoom*> rooms;
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
                    result[j][i] = Grid::DUNGEON;
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
                    result[j][i] = Grid::DUNGEON;
                }
            }
        }
    }

    return result;
}

// connect left and right room
void Dungeon2D::Connect(Dungeon2DRoom* room)
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
            if (up[i][connectX] == Grid::DUNGEON)
            {
                connectYup = i - 1;
                break;
            }
        }

        for (int i = gridNum - 1; i >= 0; i--)
        {
            if (down[i][connectX] == Grid::DUNGEON)
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
            if (right[connectY][i] == Grid::DUNGEON)
            {
                connectXright = i - 1;
                break;
            }
        }

        for (int i = gridNum - 1; i >= 0; i--)
        {
            if (left[connectY][i] == Grid::DUNGEON)
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

void Dungeon2D::ConnectRooms(Dungeon2DRoom* root)
{
    int deepestLevel = GetDeepestLevel(root);

    std::vector<Dungeon2DRoom*> rooms;

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


void Dungeon2D::GenerateStartAndGoal(Dungeon2DRoom* room)
{
    auto goalRoom = SelectRandomRoom(room);
    goalPos_ = glm::ivec2
    (
        Random::Range((int)goalRoom->GetLeftDown().x, (int)goalRoom->GetRightUp().x), Random::Range((int)goalRoom->GetLeftDown().y, (int)goalRoom->GetRightUp().y)
    );

    auto startRoom = SelectRandomRoom(room);
    playerPos_ = glm::ivec2
    (
        Random::Range((int)startRoom->GetLeftDown().x, (int)startRoom->GetRightUp().x), Random::Range((int)startRoom->GetLeftDown().y, (int)startRoom->GetRightUp().y)
    );
}


Dungeon2DRoom* Dungeon2D::GenerateDungeon()
{
    // divide rooms
    Dungeon2DRoom* root = GenerateDividedRoom();

    // add padding
    GeneratePaddingToRooms(root);

    // connect rooms
    ConnectRooms(root);

    // generate start and goal
    GenerateStartAndGoal(root);

    return root;
}

std::vector<std::vector<int>> Dungeon2D::GenerateDungeonToData()
{
    //TODO cleanup room
    auto root = GenerateDungeon();

    auto dungeonData = ConvertRoomToVector2D(root);

    // add player and goal
    dungeonData[playerPos_.y][playerPos_.x] = Grid::PLAYER;
    dungeonData[goalPos_.y][goalPos_.x] = Grid::GOAL;

    return dungeonData;
}

void Dungeon2D::DisplayDungeonData(std::vector<std::vector<int>> dungeonData)
{
    for (int i = 0; i < gridNum; i++)
    {
        for (int j = 0; j < gridNum; j++)
        {
            float x = j * gridScale + gridScale * 0.5f;
            float y = i * gridScale + gridScale * 0.5f;
            if (dungeonData[i][j] == Grid::DUNGEON)
            {
                ShapeDrawer::Rect(Vec2(x, y), Vec2(gridScale * 0.95f), Vec4(0.1f, 0.1f, 0.1f, 0.6f));
            }
            else if (dungeonData[i][j] == Grid::PLAYER)
            {
                ShapeDrawer::Triangle(Vec2(x, y), Vec2(playerDir_.x, playerDir_.y), Vec2(gridScale * 1.0f, gridScale * 0.7f), Vec4(0.7f, 0.0f, 0.0f, 1.0f));
                ShapeDrawer::Rect(Vec2(x, y), Vec2(gridScale * 0.95f), Vec4(0.1f, 0.1f, 0.1f, 0.6f));
            }
            else if (dungeonData[i][j] == Grid::GOAL)
            {
                Text("G", Vec2(x, y), gridScale * 30.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
                ShapeDrawer::Rect(Vec2(x, y), Vec2(gridScale * 0.9), Vec4(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }

}


// return goal or not
void Dungeon2D::MovePlayer(std::vector<std::vector<int>>& dungeonData, glm::ivec2 direction)
{
    auto nextPos = playerPos_ + direction;
    if (nextPos.x < 0 || nextPos.x >= gridNum || nextPos.y < 0 || nextPos.y >= gridNum) return;
    if (dungeonData[nextPos.y][nextPos.x] == Grid::EMPTY) return;

    dungeonData[playerPos_.y][playerPos_.x] = Grid::DUNGEON;
    dungeonData[nextPos.y][nextPos.x] = Grid::PLAYER;

    playerPos_ = nextPos;
    playerDir_ = direction;
}



void Dungeon2D::Start(shared_ptr<Engine> engine)
{
    Debug::Log("Dungeon2D::Start()");

    dungeon_ = GenerateDungeonToData();
    currentFloor_ = 1;
}


bool Dungeon2D::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    bool closeScene = false;


    engine->BeginRenderToScreen();

    if (Input::KeyDownNow(InputCode::Key::R))
    {
        //TODO cleanup room before regenerate
        dungeon_ = GenerateDungeonToData();
    }


    if (Input::KeyDownNow(InputCode::Key::Up) || Input::KeyDownNow(InputCode::Key::W)) MovePlayer(dungeon_, glm::uvec2(0, -1));
    if (Input::KeyDownNow(InputCode::Key::Down) || Input::KeyDownNow(InputCode::Key::S)) MovePlayer(dungeon_, glm::uvec2(0, 1));
    if (Input::KeyDownNow(InputCode::Key::Left) || Input::KeyDownNow(InputCode::Key::A)) MovePlayer(dungeon_, glm::uvec2(-1, 0));
    if (Input::KeyDownNow(InputCode::Key::Right) || Input::KeyDownNow(InputCode::Key::D)) MovePlayer(dungeon_, glm::uvec2(1, 0));


    if (playerPos_ == goalPos_)
    {
        currentFloor_++;
        dungeon_ = GenerateDungeonToData();
    }


    Text(to_string(currentFloor_) + " F", Vec2(0.05f, 0.95f), 2.0f);
    DisplayDungeonData(dungeon_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Dungeon2D::Release()
{
    Debug::Log("Dungeon2D::Release()");
}

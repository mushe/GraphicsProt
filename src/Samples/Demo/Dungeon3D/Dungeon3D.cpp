#include "Core/Engine.h"
#include "Dungeon3D.h"
#include <cmath>

using namespace Dungeon3DConstants;

void Dungeon3D::GetAllRooms(Dungeon3DRoom* room, std::vector<Dungeon3DRoom*>& rooms)
{
    if (room == nullptr) return;

    rooms.push_back(room);

    if (!room->IsLeaf())
    {
        if (room->GetLeft() != nullptr) GetAllRooms(room->GetLeft(), rooms);
        if (room->GetRight() != nullptr) GetAllRooms(room->GetRight(), rooms);
    }
}

void Dungeon3D::GetAllRoomsAtLevel(Dungeon3DRoom* room, int level, std::vector<Dungeon3DRoom*>& rooms)
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

void Dungeon3D::GetAllLeaves(Dungeon3DRoom* room, std::vector<Dungeon3DRoom*>& rooms)
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

Dungeon3DRoom* Dungeon3D::SelectRandomRoom(Dungeon3DRoom* dungeon)
{
    std::vector<Dungeon3DRoom*> rooms;
    GetAllLeaves(dungeon, rooms);
    return rooms[Random::Range(0, rooms.size() - 1)];
}

void Dungeon3D::LogRoom(Dungeon3DRoom* room)
{
    if (room == nullptr) return;

    Debug::Log("leftDown: " + to_string(room->GetLeftDown().x) + ", " + to_string(room->GetLeftDown().y) + " rightUp: " + to_string(room->GetRightUp().x) + ", " + to_string(room->GetRightUp().y));

}

void Dungeon3D::DisplayRoom(Dungeon3DRoom* room)
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

void Dungeon3D::DisplayRooms(Dungeon3DRoom* rootRoom)
{
    std::vector<Dungeon3DRoom*> leaves;
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

int Dungeon3D::GetDeepestLevel(Dungeon3DRoom* room)
{
    if (room == nullptr) return 0;

    auto leaves = std::vector<Dungeon3DRoom*>();
    GetAllLeaves(room, leaves);

    int deepestLevel = 0;
    for (auto leaf : leaves)
    {
        int l = leaf->GetLevel();
        if (l > deepestLevel) deepestLevel = l;
    }
    return deepestLevel;
}


Dungeon3DRoom* Dungeon3D::GenerateDividedRoom()
{
    Dungeon3DRoom* root = new Dungeon3DRoom(initialLeftDown, initialRightUp, nullptr, 0);
    root->Divide();
    for (int i = 0; i < maxDivision; i++)
    {
        auto leaves = std::vector<Dungeon3DRoom*>();
        GetAllLeaves(root, leaves);

        auto dividableLeaves = std::vector<Dungeon3DRoom*>();
        for (auto leaf : leaves)
        {
            if (leaf->IsDividable()) dividableLeaves.push_back(leaf);
        }

        if (dividableLeaves.size() == 0) break;

        dividableLeaves[Random::Range(0, dividableLeaves.size() - 1)]->Divide();
    }

    return root;
}

void Dungeon3D::GeneratePaddingToRooms(Dungeon3DRoom* root)
{
    std::vector<Dungeon3DRoom*> leaves;
    GetAllLeaves(root, leaves);

    for (auto leaf : leaves)
    {
        leaf->GeneratePadding();
    }
}

std::vector<std::vector<int>> Dungeon3D::ConvertRoomToVector2D(Dungeon3DRoom* root)
{
    std::vector<std::vector<int>> result = std::vector<std::vector<int>>(gridNum, std::vector<int>(gridNum, 0));

    std::vector<Dungeon3DRoom*> rooms;
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
void Dungeon3D::Connect(Dungeon3DRoom* room)
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

void Dungeon3D::ConnectRooms(Dungeon3DRoom* root)
{
    int deepestLevel = GetDeepestLevel(root);

    std::vector<Dungeon3DRoom*> rooms;

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


void Dungeon3D::GenerateStartAndGoal(Dungeon3DRoom* room)
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


Dungeon3DRoom* Dungeon3D::GenerateDungeon()
{
    // divide rooms
    Dungeon3DRoom* root = GenerateDividedRoom();

    // add padding
    GeneratePaddingToRooms(root);

    // connect rooms
    ConnectRooms(root);

    // generate start and goal
    GenerateStartAndGoal(root);

    return root;
}

std::vector<std::vector<int>> Dungeon3D::GenerateDungeonToData()
{
    //TODO cleanup room
    auto root = GenerateDungeon();

    auto dungeonData = ConvertRoomToVector2D(root);

    // add player and goal
    dungeonData[playerPos_.y][playerPos_.x] = Grid::PLAYER;
    dungeonData[goalPos_.y][goalPos_.x] = Grid::GOAL;

    return dungeonData;
}

void Dungeon3D::DisplayDungeonData(std::vector<std::vector<int>> dungeonData)
{
    for (int i = 0; i < gridNum; i++)
    {
        for (int j = 0; j < gridNum; j++)
        {
            float scaleMultiplyForMiniMap = 0.3f;
            float x = j * gridScale + gridScale * 0.5f + 2.2f;
            float y = i * gridScale + gridScale * 0.5f + 0.15f;

            x *= scaleMultiplyForMiniMap;
            y *= scaleMultiplyForMiniMap;
            if (dungeonData[i][j] == Grid::DUNGEON)
            {
                ShapeDrawer::Rect(Vec2(x, y), Vec2(gridScale) * scaleMultiplyForMiniMap, Vec4(0.1f, 0.1f, 0.1f, 1.0f));
            }
            else if (dungeonData[i][j] == Grid::PLAYER)
            {
                ShapeDrawer::Circle(Vec2(x, y), Vec2(gridScale * scaleMultiplyForMiniMap), Vec4(0.7f, 0.0f, 0.0f, 1.0f));
            }
            else if (dungeonData[i][j] == Grid::GOAL)
            {
                Text("G", Vec2(x, y), gridScale * 60.0f * scaleMultiplyForMiniMap, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
                ShapeDrawer::Rect(Vec2(x, y), Vec2(gridScale) * scaleMultiplyForMiniMap, Vec4(0.0f, 0.0f, 1.0f, 1.0f));
            }
        }
    }

}


// return goal or not
void Dungeon3D::MovePlayer(std::vector<std::vector<int>>& dungeonData, glm::ivec2 direction)
{
    if (direction.x == -1)
    {
        if (playerDir_.x == 1) playerDir_ = glm::ivec2(0, -1);
        else if (playerDir_.y == -1) playerDir_ = glm::ivec2(-1, 0);
        else if (playerDir_.x == -1) playerDir_ = glm::ivec2(0, 1);
        else if (playerDir_.y == 1) playerDir_ = glm::ivec2(1, 0);
    }
    else if (direction.x == 1)
    {
        if (playerDir_.x == 1) playerDir_ = glm::ivec2(0, 1);
        else if (playerDir_.y == 1) playerDir_ = glm::ivec2(-1, 0);
        else if (playerDir_.x == -1) playerDir_ = glm::ivec2(0, -1);
        else if (playerDir_.y == -1) playerDir_ = glm::ivec2(1, 0);
    }

    auto nextPos = playerPos_ - playerDir_ * direction.y;
    if (nextPos.x < 0 || nextPos.x >= gridNum || nextPos.y < 0 || nextPos.y >= gridNum) return;
    if (dungeonData[nextPos.y][nextPos.x] == Grid::EMPTY) return;

    dungeonData[playerPos_.y][playerPos_.x] = Grid::DUNGEON;
    dungeonData[nextPos.y][nextPos.x] = Grid::PLAYER;

    playerPos_ = nextPos;
    //playerDir = direction;
}

void Dungeon3D::Start(shared_ptr<Engine> engine)
{
    dungeon_ = GenerateDungeonToData();
    currentFloor_ = 1;
    showMap_ = true;


    // create mesh
    mesh_ = Mesh::FromOBJ("../Models/Cube/Cube.obj", 4);
    //auto mesh = Mesh::FromOBJ("../Models/Grass/Grass.obj", 4);
    tex_ = Texture::FromPNG("../Textures/DungeonFloor.png");
    
    mat_ = Material::Create(
        "dungeon.vert", "dungeon.frag",
        sizeof(DungeonUBO),
        sizeof(DungeonInstancingBuffer) * instanceCount,
        { tex_ }
    );

    instancingBuffer_.resize(instanceCount);

    for (int i = 0; i < instanceCount; ++i)
    {
        instancingBuffer_[i].pos = Vec3(0);
        instancingBuffer_[i].type = Random::Range(0, 1);
    }
    mat_->SetInstancingUniformBuffer(instancingBuffer_.data());

    // create custom uniform buffer
    mat_->SetUniformBufferData(&ubo_);
    mesh_->SetMaterial(mat_);

    
    OrbitalControl::SetOrbit(1.56f, 3.71f, 0.039f);
    OrbitalControl::Update(camera_, 0.0f, 0.0f);

}


bool Dungeon3D::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    // dungeon movement input
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
    if (Input::KeyDownNow(InputCode::Key::M)) showMap_ = !showMap_;

    OrbitalControl::SetPhi(atan2(playerDir_.x, -playerDir_.y) + 0.57f);
    OrbitalControl::Update(camera_, 0.0f, 0.0f);

    bool closeScene = false;


    engine->BeginRenderToScreen();

    // draw 3d meshes
    int index = 0;
    Vec3 cameraOffs = Vec3(playerPos_.x, yOffs_, playerPos_.y);
    for (int i = 0; i < gridNum; i++)
    {
        for (int j = 0; j < gridNum; j++)
        {
            Vec3 posOffs = Vec3(0);
            if (dungeon_[i][j] == Grid::EMPTY) posOffs = Vec3(0.0f, 1.0f, 0.0f);

            instancingBuffer_[index].pos = Vec3((float)j, 0, (float)i) - cameraOffs + posOffs;
            instancingBuffer_[index].type = dungeon_[i][j];
            index++;
        }
    }
    mesh_->Draw(camera_, instanceCount);

    Text(to_string(currentFloor_) + " F", Vec2(0.05f, 0.95f), 2.0f);
    if (showMap_) DisplayDungeonData(dungeon_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void Dungeon3D::Release()
{
}

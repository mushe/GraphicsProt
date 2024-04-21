#include "Core/Engine.h"

int main()
{
    // engine initialization
    auto engine = Engine::Init();

    // create mesh from obj
    auto mesh = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh->SetPosition(Vec3(0, -1.0f, 0));
    mesh->SetRotation(Vec3(0));
    mesh->SetScale(Vec3(0.1f));

    // load texture
    auto tex = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");

    // create material by shader and texture
    auto mat = Material::Create("Standard.vert", "UnlitTexture.frag", { tex });

    // set material to mesh
    mesh->SetMaterial(mat);

    // create camera for object rendering
    Camera camera;

    // main rendering loop
    while (engine->Running())
    {
        engine->BeginFrame(); // must be called at the beginning of every frame

        // input handling
        if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

        // camera orbital control with mouse
        OrbitalControl::Update(camera);

        // object rendering to screen by camera
        engine->BeginRenderToScreen();
        mesh->Draw(camera);
        engine->EndRenderToScreen();

        engine->EndFrame(); // must be called at the end of every frame
    }

    engine->Terminate();
}

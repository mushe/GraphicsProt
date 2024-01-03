# About

GraphicsProt is a simple and straightforward graphics prototyping(programming) framework for C++ and Vulkan, featuring sample implementations.

- Windows & Mac cross platform
- C++ & GLSL code only (no editor)
- game engine like easy API, short code
- prototyping purpose only (currently not support for production)
- including sample implementations for my research / hobby purpose (this is main focus)

# Installation

- Windows : CMake and shader compilation by executing Generate_Windows_VS2022.bat (more details later)
- Mac : more details later

# Dependencies

C++(20), [Vulkan](https://www.vulkan.org/), [GLFW](https://github.com/glfw/glfw), [ImGUI](https://github.com/ocornut/imgui), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h), [glm](https://github.com/g-truc/glm)

# Samples

## Minimum Code
![image](https://github.com/mushe/GraphicsProt/assets/26865534/af7f4c0c-8673-47ce-8c76-0b11312417b2)
minimum code to draw .obj
- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/01_Minimum
- 3D model credit
"Steven Universe House" by Zypheos : https://sketchfab.com/3d-models/steven-universe-house-ea77c49785fc4802b697bde0f7cfda8f

----

## Gerstner Waves

![GIF 1-1-2024 11-32-54 PM](https://github.com/mushe/GraphicsProt/assets/26865534/a2ceeaff-14d0-46bf-90ce-20f1a58a5a45)

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/02_GerstnerWaves
- Demo : https://www.youtube.com/watch?v=WFtOAE4n0-w

Gerstner Waves is a famous algorithm that draws pseudo ocean

background texture is generated by DreamStudio

- References

(1) GPU Gems Chapter 1. Effective Water Simulation from Physical Models

https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models

(2) 【ポケットモンスター スカーレット・バイオレット】 パルデア地方を描き出す――見た目の仕組みを徹底解説！

https://cedec.cesa.or.jp/2023/session/detail/s64242ce14adbf

----

## Post Process (Grayscale)
![image](https://github.com/mushe/GraphicsProt/assets/26865534/56326ed8-e1cf-43c0-abee-7f4968bb7182)

simple grayscale post process implementation

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/03_GrayScale

- References

(1) Grayscale - Wikipedia

https://en.wikipedia.org/wiki/Grayscale

----

## Grass
![GIF 1-2-2024 4-39-25 AM](https://github.com/mushe/GraphicsProt/assets/26865534/9110e6e3-2f9d-42bb-8e81-b3f5bb641f4b)

grass texture is generated by DreamStudio

floor texture is created at Test Grid Generator : https://wahooney.itch.io/texture-grid-generator

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/04_Grass

- References

(1) UnrelVFXCollection - Grass

https://github.com/mushe/UnrealVFXCollection/tree/main/Content/VFXCollections/Grass

----

## Normal Mapping from Texture (diffuse and specular)
![image](https://github.com/mushe/GraphicsProt/assets/26865534/61cc17e5-70c7-4061-841d-36ff009bacf0)

normal map texture is generated at https://cpetry.github.io/NormalMap-Online/

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/05_NormalMapping

# Usage Examples

## minimum code

```cpp
#include "Core/Engine.h"

int main()
{
    // engine initialization
    auto engine = Engine::Init();

    // create mesh from obj
    auto mesh = Mesh::FromOBJ("../Models/StevenUNiverseRoom/StevenUNiverseRoom.obj");
    mesh->SetPosition(glm::vec3(0, -1.0f, 0));
    mesh->SetRotation(glm::vec3(0));
    mesh->SetScale(glm::vec3(0.1f));

    // load texture
    auto tex = Texture::FromPNG("../Models/StevenUNiverseRoom/StevenUNiverseRoom.png");

    // create material by shader and texture
    auto mat = Material::Create("standard.vert", "unlitTexture.frag", { tex });

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
```

## object rendering to RenderTexture

```cpp
auto rt = RenderTarget::Create(1920, 1080);
...
rt->BeginRenderToTexture();
mesh->Draw(camera);
rt->EndRenderToTexture();
```

## draw(Blit) material to RenderTexture

post process etc.. implemented using this feature

```cpp

engine->BlitToRenderTarget(material, rt);
...
engine->BlitToScreen(material);
```

## GLSL shader loading

```cpp
// create material by shader and texture
auto mat = Material::Create("standard.vert", "unlitTexture.frag", { tex });
```

## pass uniform buffer to shader

```cpp
PostProcessUBO ubo{};
material->SetUniformBufferData(&ubo);
```

## .obj loading

```cpp
auto mesh = Mesh::FromOBJ("../path/to/obj/X.obj");
```

## .png loading

```cpp
auto tex = Texture::FromPNG("../path/to/png/X.png");
```

## GUI

imgui accessor

```jsx
engine->OnGUI([&]()
{
    GUI::Float(scale_, "scale", 0.1f);
		GUI::FloatSlider(colorLerp, "colorLerp", 0.0f, 1.0f);
    GUI::Color(color_, "color_");
    GUI::Vector(g_objPos, "g_objPos", 0.01f);
    GUI::VectorSlider(lightDir, "lightDir", -1.0f, 1.0f);
});
```

## key and mouse input

```cpp
// key handling
if (Input::KeyDown(KeyCode::Escape)) engine->Quit();

// mouse drag delta
glm::vec2 rightMouseDrag = Input::LeftMouseDrag();

// mouse sheel delta
float wheelVel = Input::MouseWheelDelta();
```

## orbital camera control with mouse

```cpp
Camera camera;
OrbitalControl::Update(camera);
```

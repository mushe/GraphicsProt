# About

GraphicsProt is a simple and straightforward graphics prototyping(programming) framework for C++ and Vulkan, featuring sample implementations.

- Windows & Mac cross platform
- C++ & GLSL code only (no editor)
- game engine like easy API, short code
- prototyping purpose only (currently not support for production)
- including sample implementations for my research / hobby purpose (this is main focus)

# Installation

- Windows : CMake and shader compilation by executing Generate_Windows_VS2022.bat (more details later)
- Mac : CMake and Visual Studio Code (more details later)

# Dependencies

C++(17), [Vulkan](https://www.vulkan.org/), [GLFW](https://github.com/glfw/glfw), [ImGUI](https://github.com/ocornut/imgui), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h), [glm](https://github.com/g-truc/glm)

# Samples

## Minimum Code
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/e7f6ebde-0fe2-4674-b6e1-b37c706c347a" width="70%"></p>
minimum code to draw .obj

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Minimum
- 3D model credit
"Steven Universe House" by Zypheos : https://sketchfab.com/3d-models/steven-universe-house-ea77c49785fc4802b697bde0f7cfda8f

## Gerstner Waves
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/c29f756a-55de-49d5-8a82-4989c690f241" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/GerstnerWaves
- Demo : https://www.youtube.com/watch?v=WFtOAE4n0-w

Gerstner Waves is a famous algorithm that draws pseudo ocean

background texture is generated by DreamStudio

- References

(1) GPU Gems Chapter 1. Effective Water Simulation from Physical Models

https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models

(2) 【ポケットモンスター スカーレット・バイオレット】 パルデア地方を描き出す――見た目の仕組みを徹底解説！

https://cedec.cesa.or.jp/2023/session/detail/s64242ce14adbf

## Grayscale (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/3bd988ff-6732-4971-a491-7777e03b5e11" width="70%"></p>

simple grayscale post process implementation

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/GrayScale

- References

(1) Grayscale - Wikipedia

https://en.wikipedia.org/wiki/Grayscale

## Grass
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/24a3029f-2a54-49a4-b2c8-52fd6c969479" width="70%"></p>

grass texture is generated by DreamStudio

floor texture is created at Test Grid Generator : https://wahooney.itch.io/texture-grid-generator

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Grass

- References

(1) UnrelVFXCollection - Grass

https://github.com/mushe/UnrealVFXCollection/tree/main/Content/VFXCollections/Grass

## Normal Mapping from Texture (diffuse and specular)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/b0119761-b24c-413e-b772-e889ddfaa2fe" width="70%"></p>

normal map texture is generated at https://cpetry.github.io/NormalMap-Online/

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/NormalMapping

## Instancing
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/c1e7763a-a5e3-4eed-a483-a3116b49ca74" width="70%"></p>

Instancing is a standard technique for drawing many objects
- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Instancing

## Drawing Basic 2D Shape (Instancing)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/f1b425ca-2795-4480-ade5-08fe85e200e9" width="70%"></p>

drawing shapes(rect, circle, triangle, line) with a handy API

```cpp
engine->BeginRenderToScreen();

// call between BeginRenderToScreen and EndRenderToScreen
ShapeDrawer::Rect(Vec2(0.25), Vec2(0.25)); // position, scale, (color)
ShapeDrawer::Circle(Vec2(0.75), Vec2(0.25), Vec4(1)); // position, scale, (color)
ShapeDrawer::Line(Vec2(0.625, 0.125), Vec2(0.875, 0.375)); // start, end, (width), (color)
ShapeDrawer::Triangle(Vec2(0.25, 0.75), Vec2(0.25), Vec2(0.25)); // position, direction, scale, (color)

engine->EndRenderToScreen();
```

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Shape

----

## Maze Generation
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/a2f7295e-0ada-4ae1-8bc7-684a5f501cef" width="70%"></p>

procedurally generated maze
- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Maze

- References

(1) 自動生成迷路

https://www5d.biglobe.ne.jp/~stssk/maze/make.html

## Pi estimation by Monte Carlo Method
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/aed70674-19b9-4fdd-a67f-cd3e1781d81d" width="70%"></p>

Plot points at random against some location in the circle circumscribed by the square. Then the number of points in the circle / number of points in the square = π / 4, which gets closer as the number of points increases. Using this logic, we can approximate pi.

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/PiCalculation

## Chromatic Aberration (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/589abaae-4f02-4bfc-96cf-b00de3024d12" width="70%"></p>

Shift the RGB color pick position for each RGB individually

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/ChromaticAberration

## Halftone by probability (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/b902e35e-fd31-47f7-8cf1-4457a3071cff" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/HalfTone
- References

(1) コンピュータグラフィックス編集委員会, "コンピュータグラフィックス", 画像情報教育振興協会, 2008, p199.

## Posterization (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/97eae314-8723-4c73-ab2c-9d2c4012a517" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Posterization
- Image Credit : https://pixabay.com/photos/mountain-nature-landscape-picture-646389/
- References

(1) コンピュータグラフィックス編集委員会, "コンピュータグラフィックス", 画像情報教育振興協会, 2008, p221.

## Pseudo Color (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/447ea9b8-311a-4ade-a431-c4ebb6b7b476" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/PseudoColor
- Image Credit : https://en.wikipedia.org/wiki/File:Milwaukee_City_Hall_Old_Public_Domain_Photo.jpg
- References

(1) コンピュータグラフィックス編集委員会, "コンピュータグラフィックス", 画像情報教育振興協会, 2008, p224.

## Sepia (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/649d28ec-f5c2-49a7-8314-9d89685eb833" width="70%"></p>

nostalgic post process

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Sepia
- References

(1) How to convert a color image into sepia image - Image Processing Project - dyclassroom

https://dyclassroom.com/image-processing-project/how-to-convert-a-color-image-into-sepia-image

## Text Rendering (instancing)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/ae65c562-2408-49d1-a9f2-bda8fa6c51c0" width="70%"></p>

handy text rendering API

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/TextRender
- Font Credit :  https://levien.com/type/myfonts/inconsolata.html

## White Noise (CPU)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/8319a337-b9f1-468f-a485-c1a575919057" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/WhiteNoiseCPU

## White Noise (GPU)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/b097bab1-cc50-4b67-81da-1ae49047dfdb" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/WhiteNoiseGPU

## Catmull–Rom Spline interpolation
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/e92b686e-a45f-4e06-b59b-269a82647876" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/CatmullRomSpline
- References

(1) Catmull–Rom Spline補間

https://zenn.dev/mushe/articles/92c65e0c8023aa

## Kaleidoscope
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/3e7ab265-dd03-4d61-8b9a-8af320396e06" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Kaleidoscope
- References

(1) mushe/UnrealVFXCollection:

https://github.com/mushe/UnrealVFXCollection

## Kira sticker
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/13bcd81f-dd81-4c73-bcef-bd65288bb359" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/KiraSticker
- References

(1) mushe/UnrealVFXCollection:

https://github.com/mushe/UnrealVFXCollection

## Sierpiński triangle
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/b2d99945-7cfc-47e4-b832-f9712497d837" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/SierpinskiTriangle
- References

(1) Sierpiński triangle - Wikipedia

[https://en.wikipedia.org/wiki/Sierpiński_triangle](https://en.wikipedia.org/wiki/Sierpi%C5%84ski_triangle)

## Koch snowflake
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/45e93dc7-b9bb-43de-8cf3-feeecdc93305" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/KochSnowflake
- References

(1) Koch snowflake - Wikipedia

https://en.wikipedia.org/wiki/Koch_snowflake

## Hover
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/52bb6337-0db3-4922-9091-d03935dd4244" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Hover
- References

(1) UnrelVFXCollection - HoverGame

https://github.com/mushe/UnrealVFXCollection

## Boids 2D (CPU)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/bbfcb4db-3a39-4923-95aa-3e2216052de1" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Boids2dCPU
- References

(1) Flocks, herds and schools: A distributed behavioral model

https://dl.acm.org/doi/10.1145/37402.37406

## Perlin Noise (GPU)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/95e55134-5da6-496c-85ca-147e1ed60b87" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/PerlinNoise
- References

https://gist.github.com/mushe/17acdb2b90db663948cddb1a0f21e490

## Room Division
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/df410de0-18d7-4c8e-8d47-239b8a9ef507" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/RoomDivision
- References

(1) Basic BSP Dungeon generation

https://www.roguebasin.com/index.php/Basic_BSP_Dungeon_generation

## Dungeon Generation
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/b0897120-8d05-41cf-95c8-d12d683b8dec" width="70%"></p>

wall texture is generated by stable diffusion

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/DungeonGeneration
- References

(1) Basic BSP Dungeon generation

https://www.roguebasin.com/index.php/Basic_BSP_Dungeon_generation

## Dungeon Walk 2D
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/c9ef6e29-93bc-470c-9c0a-4e2d71d419bf" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Dungeon2D
- References

(1) Basic BSP Dungeon generation

https://www.roguebasin.com/index.php/Basic_BSP_Dungeon_generation

## Dungeon Walk 3D
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/96ab5750-77cf-4726-8a29-1d451b714e07" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Dungeon3D
- References

(1) Basic BSP Dungeon generation

https://www.roguebasin.com/index.php/Basic_BSP_Dungeon_generation

## VHS (post process)
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/6bdbdf21-32b7-45e0-8633-466b98798f42" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/VHS
- References

(1) UnrelVFXCollection - VHS

https://github.com/mushe/UnrealVFXCollection

## River
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/aabe350f-6afa-45cc-89e1-90c879b6f7a4" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/River
- References

(1) UnrelVFXCollection - SimpleRiver

https://github.com/mushe/UnrealVFXCollection

## Poisson Disk Sampling
<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/944adc49-0a64-49e8-b390-622898c54ef5" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/PoissonDiskSampling
- References :

(1) Fast Poisson Disk Sampling in Arbitrary Dimensions

https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf

## Fish Swarm (compute shader)

<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/da81d45c-2f11-4745-9e65-7bca1f68d44b" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Fish
- References :

(1) Reynolds, Craig (1987). Flocks, herds and schools: A distributed behavioral model. SIGGRAPH '87: Proceedings of the 14th Annual Conference on Computer Graphics and Interactive Techniques.

(2) UnrelVFXCollection - Flocking

https://github.com/mushe/UnrealVFXCollection

## Blur (post process)

<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/582f4977-9d84-4a8d-9163-5e8041bf5383" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Blur
- References :

(1) Marius Bjørge(2015), Bandwidth-Efficient Rendering, SIGGRAPH 2015

https://community.arm.com/cfs-file/__key/communityserver-blogs-components-weblogfiles/00-00-00-20-66/siggraph2015_2D00_mmg_2D00_marius_2D00_slides.pdf

## Fluid Simulation and Water Rendering

<p align="center"><img src="https://github.com/mushe/GraphicsProt/assets/26865534/c418c297-446b-47a4-b027-4beea8f4c7be" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/FluidSim
- References :

(1) Doyub Kim, "Fluid Engine Development", A K Peters/CRC Press, 2017

https://www.amazon.co.jp/-/en/Doyub-Kim-ebook/dp/B01MRDA6S8

(2) Matthias Müller et al, "Particle-based fluid simulation for interactive applications", ACM SIGGRAPH/Eurographics symposium on Computer animation, 2003

https://dl.acm.org/doi/10.5555/846276.846298

(3) Screen Space Fluid Rendering for Games - Nvidia

https://developer.download.nvidia.com/presentations/2010/gdc/Direct3D_Effects.pdf

(4) 武者 拓也, "Unreal Engine 5で学ぶビジュアルエフェクト実装 基本機能からNiagara、シミュレーションまで", 翔泳社, 2023

https://www.shoeisha.co.jp/book/detail/9784798177700


## Sieve of Eratosthenes

<p align="center"><img src="https://github.com/user-attachments/assets/36583736-7d7e-42e8-9217-f5a32874def5" width="70%"></p>

- Code : https://github.com/mushe/GraphicsProt/tree/main/src/Samples/Demo/Eratosthenes


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

## text drawing
```cpp
// text, position, scale, color
Text("ABC", Vec2(0.5f, 0.5f), 1.0f, Vec4(1,1,1,1));
```

## 2d shape drawing
```cpp
engine->BeginRenderToScreen();

// call between BeginRenderToScreen and EndRenderToScreen
ShapeDrawer::Rect(Vec2(0.25), Vec2(0.25)); // position, scale, (color)
ShapeDrawer::Circle(Vec2(0.75), Vec2(0.25), Vec4(1)); // position, scale, (color)
ShapeDrawer::Line(Vec2(0.625, 0.125), Vec2(0.875, 0.375)); // start, end, (width), (color)
ShapeDrawer::Triangle(Vec2(0.25, 0.75), Vec2(0.25), Vec2(0.25)); // position, direction, scale, (color)

engine->EndRenderToScreen();
```

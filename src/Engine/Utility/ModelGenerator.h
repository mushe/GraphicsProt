#pragma once
#include "Core/Mesh.h"
#include "Core/Material.h"

struct ModelGeneratorData
{
    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;
};

class ModelGenerator
{
public:
    static ModelGeneratorData GeneratePlane(int division = 1);
    static ModelGeneratorData GenerateDisplayQuad();
};

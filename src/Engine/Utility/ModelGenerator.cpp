#include "ModelGenerator.h"

ModelGeneratorData ModelGenerator::GeneratePlane(int division)
{
    glm::vec3 color = { 0.0f, 0.0f, 0.0f };
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
    glm::vec3 tangent = { 1.0f, 0.0f, 0.0f };
    glm::vec3 bitangent = { 0.0f, 0.0f, 1.0f };

    float gridScale = 1.0f / (float)division;
    std::vector<VertexData> vertices;
    for (int y = 0; y < division + 1; ++y)
    {
        for (int x = 0; x < division + 1; ++x)
        {
            // pos, color, normal, tangent, bitangent, texCoord
			vertices.push_back
            ({ 
                { x * gridScale - 0.5f, 0.0f, y * gridScale - 0.5f }, 
                color, 
                normal,
                tangent,
                bitangent,
                { x * gridScale, y * gridScale } 
            });
		}
	}

    std::vector<uint32_t> indices;
    for (int y = 0; y < division; ++y)
    {
        for (int x = 0; x < division; ++x)
        {
			int index = y * (division + 1) + x;
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + division + 1);
			indices.push_back(index + division + 1);
			indices.push_back(index + 1);
			indices.push_back(index + division + 2);
		}
	}

    return {vertices, indices};
}

ModelGeneratorData ModelGenerator::GenerateDisplayQuad()
{
    std::vector<VertexData> vertices =
    {
        // pos, color, normal, tangent, bitangent, texCoord
        { 
            {-1.0f, -1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f} 
        },
        { 
            {1.0f, -1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f} 
        },
        { 
            {1.0f, 1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f} 
        },
        { 
            {-1.0f, 1.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f}, 
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f} 
        }
    };
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0, };
    return { vertices, indices };
}

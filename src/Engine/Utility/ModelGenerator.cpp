#include "ModelGenerator.h"

ModelGeneratorData ModelGenerator::GenerateCube()
{
    std::vector<VertexData> vertices =
    {
        // 0
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f}
        },
        // 1
        {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f}
        },
        // 2
        {
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f}
        },
        // 3
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f}
        },


        // 4
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f}
        },
        // 5
        {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f}
        },
        // 6
        {
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f}
        },
        // 7
        {
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f}
        },



        // 8
        {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 9
        {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 10
        {
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 11
        {
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },



        // 12
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 13
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 14
        {
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 15
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}
        },

        // 16
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 17
        {
            {1.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 18
        {
            {1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 19
        {
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, 0.0f}
        },


        // 20
        {
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 21
        {
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 22
        {
            {0.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        // 23
        {
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f}
        },
    };
    std::vector<uint32_t> indices =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8,9,10, 10, 11, 8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20,
    };

    return {vertices, indices};
}

ModelGeneratorData ModelGenerator::GeneratePlane(int division)
{
    glm::vec3 color = { 0.0f, 0.0f, 0.0f };
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    float gridScale = 1.0f / (float)division;
    std::vector<VertexData> vertices;
    for (int y = 0; y < division + 1; ++y)
    {
        for (int x = 0; x < division + 1; ++x)
        {
			vertices.push_back({ {x * gridScale - 0.5f, 0.0f, y * gridScale - 0.5f}, color, normal, {x * gridScale, y * gridScale} });
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
        { {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f} },
        { {1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
        { {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f} },
        { {-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f} }
    };
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0, };
    return { vertices, indices };
}

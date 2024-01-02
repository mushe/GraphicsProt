#include "ObjParser.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "Core/Material.h"

ObjData ObjParser::Parse(std::string objPath, int face)
{
    std::ifstream file(objPath);
    std::string line;

    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    int i = 0;

    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        std::string token;

        // v
        if (line.substr(0, 2) == "v ") 
        {
            float x, y, z;
            if (iss >> token >> x >> y >> z) 
            {
                glm::vec3 pos(x, y, z);
                positions.push_back(pos);
            }
        }
        // vn
        else if (line.substr(0, 3) == "vn ") 
		{
			float x, y, z;
			if (iss >> token >> x >> y >> z) 
			{
				normals.push_back(glm::vec3(x, y, z));
			}
		}
        // vt
        else if (line.substr(0, 3) == "vt ")
        {
			float x, y;
			if (iss >> token >> x >> y)
			{
				texCoords.push_back(glm::vec2(x, y));
			}
        }
        // f
        else if (line.substr(0, 2) == "f " && face == 3)
        {
            std::string v1, v2, v3;
            uint32_t p1, p2, p3;
            uint32_t n1 = 1, n2 = 1, n3 = 1;
            uint32_t t1, t2, t3;
            if (iss >> token >> v1 >> v2 >> v3)
            {
                std::istringstream iss1(v1);
                std::istringstream iss2(v2);
                std::istringstream iss3(v3);

                iss1 >> p1; iss1.ignore(1); iss1 >> t1; iss1.ignore(1); iss1 >> n1;
                iss2 >> p2; iss2.ignore(1); iss2 >> t2; iss2.ignore(1); iss2 >> n2;
                iss3 >> p3; iss3.ignore(1); iss3 >> t3; iss3.ignore(1); iss3 >> n3;

                indices.push_back(i);
                indices.push_back(i + 1);
                indices.push_back(i + 2);
                i += 3;

                // pos, color, normal, tangent, bitangent, texCoord
                VertexData vertex1
                { 
                    positions[p1 - 1], 
                    glm::vec3(0.0, 0.0, 0.0), 
                    normals[n1 - 1], 
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t1 - 1] 
                };
                VertexData vertex2
                { 
                    positions[p2 - 1], 
                    glm::vec3(0.0, 0.0, 0.0), 
                    normals[n2 - 1], 
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t2 - 1] 
                };
                VertexData vertex3
                { 
                    positions[p3 - 1], 
                    glm::vec3(0.0, 0.0, 0.0), 
                    normals[n3 - 1], 
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t3 - 1]
                };
                vertices.push_back(vertex1);
                vertices.push_back(vertex2);
                vertices.push_back(vertex3);
            }
        }
        // f
        else if (line.substr(0, 2) == "f " && face == 4)
        {
            std::string v1, v2, v3, v4;
			uint32_t p1, p2, p3, p4;
            uint32_t n1 = 1, n2 = 1, n3 = 1, n4 = 1;
			uint32_t t1, t2, t3, t4;
            if (iss >> token >> v1 >> v2 >> v3 >> v4)
            {
                std::istringstream iss1(v1);
                std::istringstream iss2(v2);
                std::istringstream iss3(v3);
                std::istringstream iss4(v4);

                iss1 >> p1; iss1.ignore(1); iss1 >> t1; iss1.ignore(1); iss1 >> n1;
                iss2 >> p2; iss2.ignore(1); iss2 >> t2; iss2.ignore(1); iss2 >> n2;
                iss3 >> p3; iss3.ignore(1); iss3 >> t3; iss3.ignore(1); iss3 >> n3;
                iss4 >> p4; iss4.ignore(1); iss4 >> t4; iss4.ignore(1); iss4 >> n4;

                indices.push_back(i);
                indices.push_back(i+1);
                indices.push_back(i+2);

                indices.push_back(i);
                indices.push_back(i+2);
                indices.push_back(i+3);
                i += 4;

                // pos, color, normal, tangent, bitangent, texCoord
                VertexData vertex1
                {
                    positions[p1 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    normals[n1 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t1 - 1]
                };
                VertexData vertex2
                {
                    positions[p2 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    normals[n2 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t2 - 1]
                };
                VertexData vertex3
                {
                    positions[p3 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    normals[n3 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t3 - 1]
                };
                VertexData vertex4
                {
                    positions[p4 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    normals[n4 - 1],
                    glm::vec3(0.0, 0.0, 0.0),
                    glm::vec3(0.0, 0.0, 0.0),
                    texCoords[t4 - 1]
                };
                vertices.push_back(vertex1);
                vertices.push_back(vertex2);
                vertices.push_back(vertex3);
                vertices.push_back(vertex4);
            }
        }
    }

    //Debug::Log("positions : " + std::to_string(positions.size()));
    //Debug::Log("normals : " + std::to_string(normals.size()));
    //Debug::Log("texCoords : " + std::to_string(texCoords.size()));

    return {vertices, indices};
}

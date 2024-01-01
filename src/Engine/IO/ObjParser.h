#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cassert>

#include "Core/Mesh.h"
#include "Core/Material.h"

struct ObjData
{
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
};

class ObjParser
{
public:
	ObjParser() {}
	~ObjParser() {}
	ObjData Parse(std::string objPath, int face = 3);
};
#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cassert>

namespace Timer
{
    void Start(std::string name);
    void End(std::string name);
    void Clear();
}
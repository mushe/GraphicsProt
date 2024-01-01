#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cassert>

namespace StopWatch
{
    void Start();
    void EndAndLog(std::string prefix);
}
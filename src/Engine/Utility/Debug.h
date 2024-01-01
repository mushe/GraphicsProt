#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Debug
{
public:

	template<typename T>
	static std::string argsToString(T s)
	{
		std::stringstream ss;
		ss << s;
		return ss.str();
	}

	template<typename... Args>
	static void Log(Args... args)
	{
		std::stringstream ss;
		ss << argsToString(args...).c_str();
		std::cout << ss.str() << std::endl;
	}
};

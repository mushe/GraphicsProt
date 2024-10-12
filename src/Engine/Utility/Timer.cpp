#include "Timer.h"
#include <iostream>

namespace Timer
{
	std::map<std::string, clock_t> timers_;
}

void Timer::Start(std::string name)
{
	timers_[name] = clock();
}

void Timer::End(std::string name)
{
	if (timers_.find(name) == timers_.end()) 
	{
		std::cout << name << " : timer does not exist" << std::endl;
		return;
	}

	clock_t end = clock();
	double elapsed = static_cast<double>(end - timers_[name]) / CLOCKS_PER_SEC * 1000;
	std::cout << name + " : " << elapsed <<  " msec" << std::endl;
}


void Timer::Clear()
{
	timers_.clear();
}
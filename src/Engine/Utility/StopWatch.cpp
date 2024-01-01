#include "StopWatch.h"
#include <iostream>

namespace StopWatch
{
	clock_t start_;
}

void StopWatch::Start()
{
	start_ = clock();
}

void StopWatch::EndAndLog(std::string prefix)
{
	clock_t end = clock();
	const double elapsed = static_cast<double>(end - start_);
	std::cout << prefix + " : " << elapsed / CLOCKS_PER_SEC <<  " sec" << std::endl;
}
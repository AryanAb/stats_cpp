#include <iostream>
#include "Statistics.hpp"
#include "Profiler.hpp"


int main()
{
	std::vector<double> nums;
	nums.reserve(1000000);

	for (int i = 0; i < 1000000; i++)
	{
		int n = rand() % 1000000;
		nums.push_back(n);
	}

	stats::timer(&nums, stats::getOneVarStats);

	std::cout << nums.size() << std::endl;

	return 0;
}

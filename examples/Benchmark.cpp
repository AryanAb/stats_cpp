#include "../src/Statistics.hpp"
#include "../src/Profiler.hpp"

#include <vector>
#include <random>
#include <iostream>

int main()
{
	std::vector<double> nums;
	nums.reserve(1000000);

	for (int i = 0; i < 1000000; i++)
	{
		int n = rand() % 1000000;
		nums.push_back(n);
	}

	stats::timer(stats::getOneVarStats, nums);

	std::cout << nums.size() << std::endl;

	return 0;
}

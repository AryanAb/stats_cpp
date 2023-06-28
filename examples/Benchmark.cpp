#include "../src/Statistics.hpp"
#include "../src/Profiler.hpp"

#include <vector>
#include <random>
#include <iostream>

#define NUM_DATAPOINTS (500000)

int main()
{
	std::vector<double> nums;
	nums.reserve(NUM_DATAPOINTS);

	for (int i = 0; i < NUM_DATAPOINTS; i++)
	{
		int n = rand() % NUM_DATAPOINTS;
		nums.push_back(n);
	}

	stats::timer(stats::getOneVarStats, nums);

	std::cout << nums.size() << std::endl;

	return 0;
}

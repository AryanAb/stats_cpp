#include <iostream>
#include "Statistics.hpp"
#include "Profiler.hpp"

/*stats::interval stats::calcInterval(double confidence, double mean, double std)
{
	double zStar = invNormalCDF(confidence / 100);

	return nullptr;
}*/

/*void stats::timer(std::vector<double>* nums, p_getOneVarStats func)
{
	auto start = std::chrono::high_resolution_clock::now();
	func(nums);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;

	std::cout << duration.count() << std::endl;
}*/

int main()
{

	//std::cout << stats::normalCDF(1) << std::endl;
	//std::cout << stats::invNormalCDF(0.95) << std::endl;
	stats::interval inter = stats::calcInterval(0.90, 900, 0.82);
	std::cout << inter.confidence << std::endl;
	std::cout << inter.min << std::endl;
	std::cout << inter.mean << std::endl;
	std::cout << inter.max << std::endl;

	std::vector<double> nums;
	nums.reserve(100000);

	for (int i = 0; i < 100000; i++)
	{
		int n = rand() % 100000;
		nums.push_back(n);
	}

	stats::timer(&nums, stats::getOneVarStats);

	std::cout << nums.size() << std::endl;

	return 0;
}

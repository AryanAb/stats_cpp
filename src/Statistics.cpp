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

	std::vector<double> nums = {1, 2, 3, 4, 5};

	stats::oneVarStats data = stats::getOneVarStats(&nums);

	std::cout << data.mean << std::endl;

	return 0;
}

#pragma once

#include <vector>

namespace stats
{

	struct oneVarStats
	{
		double sum;
		double mean;
		double std;
		unsigned int size;
		double min;
		double q1;
		double median;
		double q3;
		double max;
		double iqr;
	};

	double simpleSum(const std::vector<double> *arr);
	double complexSum(const std::vector<double> *arr);

	oneVarStats getOneVarStats(std::vector<double> *arr);
	unsigned int median(unsigned int l, unsigned int r);
	void IQR(const std::vector<double> *arr, stats::oneVarStats *data);

	double calcZScore(double value, double mean, double std);
	double calcZScore(double value, oneVarStats stats);
	double calcZScore(double value, std::vector<double> *values);

	// calculates cdf using error function
	double normalCDF(double value);

	//using p_getOneVarStats = stats::oneVarStats(*)(std::vector<double>*);
	//void timer(std::vector<double>* nums, p_getOneVarStats func);

	template <typename T, typename F>
	void timer(T inputs, F func)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func(inputs);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;

		std::cout << duration.count() << std::endl;
	}

} // namespace stats

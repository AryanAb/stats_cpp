#ifndef SQRT_1_2
#define SQRT_1_2 0.7071067812
#endif

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <future>
#include <random>
#include <chrono>
#include "Statistics.hpp"

std::ostream& operator<<(std::ostream& stream, const stats::oneVarStats& data)
{
	stream << data.min << ", " << data.q1 << ", " << data.median << ", " << data.q3 << ", " << data.max;
	return stream;
}

stats::oneVarStats stats::getOneVarStats(std::vector<double>* arr)
{
	std::sort(arr->begin(), arr->end());

	oneVarStats data;

	data.size = arr->size();
	data.min = arr->front();
	data.max = arr->back();

	unsigned int m_index;

	if (arr->size() % 2 != 0)
	{
		m_index = (arr->size() - 1) / 2;
		data.median = arr->at(m_index);
	}
	else
	{
		data.median = (arr->at(arr->size() / 2 - 1) + arr->at(arr->size() / 2)) / 2;
	}

	stats::IQR(arr, &data);

	data.sum = stats::complexSum(arr);
	double mean = data.sum / arr->size();

	double totalDeviation = 0;
	for (double i : *arr)
	{
		totalDeviation += pow((i - mean), 2);
	}

	data.mean = mean;
	data.std = sqrt(totalDeviation / (arr->size() - 1));

	return data;
}

double stats::simpleSum(const std::vector<double>* arr)
{
	double sum = 0;
	for (double num : *arr)
	{
		sum += num;
	}

	return sum;
}

// Improved Kahan–Babuška algorithm
double stats::complexSum(const std::vector<double>* arr)
{
	double sum = 0.0;
	double c = 0.0;
	for (double num : *arr)
	{
		double t = sum + num;
		if (abs(sum) > abs(num))
		{
			c += ((sum - t) + num);
		}
		else
		{
			c += ((num - t) + sum);
		}

		sum = t;
	}
	return sum + c;
}

unsigned int stats::median(unsigned int l, unsigned int r)
{
	int n = r - l + 1;
	n = (n + 1) / 2 - 1;
	return n + l;
}

void stats::IQR(const std::vector<double>* arr, stats::oneVarStats* data)
{
	unsigned int mid_index = stats::median(0, arr->size());

	if (mid_index % 2 == 0)
	{
		unsigned int index = stats::median(0, mid_index);
		double Q1 = (arr->at(index - 1) + arr->at(index)) / 2;
		data->q1 = Q1;
	}
	else
	{
		double Q1 = arr->at(stats::median(0, mid_index));
		data->q1 = Q1;
	}

	if (mid_index % 2 == 0)
	{
		unsigned int index = stats::median(mid_index + 1, arr->size());
		double Q3 = (arr->at(index - 1) + arr->at(index)) / 2;
		data->q3 = Q3;
	}
	else
	{
		double Q3 = arr->at(stats::median(mid_index + 1, arr->size()));
		data->q3 = Q3;
	}
	data->iqr = data->q3 - data->q1;
}

double stats::calcZScore(double value, double mean, double std)
{
	return (value - mean) / std;
}
double stats::calcZScore(double value, stats::oneVarStats stats)
{
	return stats::calcZScore(value, stats.mean, stats.std);
}

double stats::calcZScore(double value, std::vector<double>* values)
{
	stats::oneVarStats data = stats::getOneVarStats(values);

	return stats::calcZScore(value, data.mean, data.std);
}

// https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
double stats::normalCDF(double value)
{
	return 0.5 * std::erfc(-value * SQRT_1_2);
}

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

	std::vector<double> nums;

	for (int i = 0; i < 1000000; i++)
	{
		nums.push_back(std::rand() % 10000);
	}

	stats::timer(&nums, stats::getOneVarStats);

	return 0;
}

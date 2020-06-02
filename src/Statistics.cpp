#ifndef SQRT_1_2
#define SQRT_1_2 0.7071067812
#endif

#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <algorithm>
#include <future>
#include <random>
#include <chrono>
#include "Statistics.hpp"
#include "Profiler.hpp"

std::ostream &operator<<(std::ostream &stream, const stats::oneVarStats &data)
{
	stream << data.min << ", " << data.q1 << ", " << data.median << ", " << data.q3 << ", " << data.max;
	return stream;
}

stats::oneVarStats stats::getOneVarStats(std::vector<double> *arr)
{
	std::sort(arr->begin(), arr->end());

	oneVarStats data;

	data.size = arr->size();
	data.min = arr->front();
	data.max = arr->back();

	size_t m_index;

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

	data.mode = stats::mode(arr);

	return data;
}

double stats::simpleSum(const std::vector<double> *arr)
{
	double sum = 0;
	for (double num : *arr)
	{
		sum += num;
	}

	return sum;
}

// Improved Kahan–Babuška algorithm
double stats::complexSum(const std::vector<double> *arr)
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

unsigned int stats::median(unsigned int l, size_t r)
{
	size_t n = r - l + 1;
	n = (n + 1) / 2 - 1;
	return n + l;
}

void stats::IQR(const std::vector<double> *arr, stats::oneVarStats *data)
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

double stats::mode(std::vector<double> *arr)
{
	std::set<double> s(arr->begin(), arr->end());

	int maxOccurances = 1;
	double mode = arr->at(0);
	for (double num : s)
	{
		int occurances = stats::count(arr, num);
		if (occurances > maxOccurances)
		{
			maxOccurances = occurances;
			mode = num;
		}
	}

	return mode;
}

int stats::count(std::vector<double> *arr, double x)
{

	std::vector<double>::iterator low = std::lower_bound(arr->begin(), arr->end(), x);

	if (low - (arr->begin()) == arr->size() || *low != x)
	{
		return 0;
	}

	std::vector<double>::iterator high = std::upper_bound(arr->begin(), arr->end(), x);

	int n = static_cast<int>((high - (arr->begin()))) - static_cast<int>((low - (arr->begin())));

	return n;
}

double stats::calcZScore(double value, double mean, double std)
{
	return (value - mean) / std;
}
double stats::calcZScore(double value, stats::oneVarStats stats)
{
	return stats::calcZScore(value, stats.mean, stats.std);
}

double stats::calcZScore(double value, std::vector<double> *values)
{
	stats::oneVarStats data = stats::getOneVarStats(values);

	return stats::calcZScore(value, data.mean, data.std);
}

const stats::linearRegression stats::calcLinearRegression(std::vector<xyPair> *nums)
{
	std::vector<double> x;
	std::vector<double> y;
	for (xyPair pair : *nums)
	{
		x.push_back(pair.x);
		y.push_back(pair.y);
	}
	stats::oneVarStats xValues = stats::getOneVarStats(&x);
	stats::oneVarStats yValue = stats::getOneVarStats(&y);

	double sum = 0;
	for (xyPair pair : *nums)
	{
		sum += stats::calcZScore(pair.x, xValues.mean, xValues.std) * calcZScore(pair.y, yValue.mean, yValue.std);
	}

	const stats::linearRegression linearRegression(sum / (nums->size() - 1), xValues, yValue);

	return linearRegression;
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

	std::vector<stats::xyPair> nums = {stats::xyPair(0, 0), stats::xyPair(1, 1), stats::xyPair(2, 2), stats::xyPair(3, 3)};

	const stats::linearRegression linearRegression = stats::calcLinearRegression(&nums);
	std::cout << linearRegression.slope << std::endl;

	return 0;
}

#pragma once

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

namespace stats
{

	struct oneVarStats
	{
		double sum;
		double mean;
		double variance;
		double std;
		size_t size;
		double min;
		double q1;
		double median;
		double q3;
		double max;
		double iqr;
		double mode;
	};

	struct xyPair
	{
		double x;
		double y;

		xyPair(double x, double y)
			: x(x), y(y)
		{
		}
	};

	struct linearRegression
	{
		double r;
		double rSquared;
		double slope;
		double yIntercept;

		linearRegression(double r, oneVarStats &x, oneVarStats &y)
			: r(r)
		{
			rSquared = r * r;
			slope = r * y.std / x.std;
			yIntercept = y.mean - slope * x.mean;
		}
	};

	struct interval
	{
		double min;
		double mean;
		double max;
		double confidence;
	};

	double simpleSum(const std::vector<double> *arr)
	{
		double sum = 0;
		for (double num : *arr)
		{
			sum += num;
		}

		return sum;
	}

	// Improved Kahan–Babuška algorithm
	double complexSum(const std::vector<double> *arr)
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

	double harmonicMean(std::vector<double> &arr)
	{
		double sum = 0;
		for (double num : arr)
		{
			sum += 1 / num;
		}

		return arr.size() / sum;
	}

	double geometricMean(std::vector<double> &arr)
	{
		double product = 1;
		for (double num : arr)
		{
			product *= num;
		}

		return std::pow(product, 1.0 / arr.size());
	}

	unsigned int median(unsigned int l, size_t r)
	{
		size_t n = r - l + 1;
		n = (n + 1) / 2 - 1;
		return n + l;
	}

	void IQR(const std::vector<double> *arr, oneVarStats *data)
	{
		unsigned int mid_index = median(0, arr->size());

		if (mid_index % 2 == 0)
		{
			unsigned int index = median(0, mid_index);
			double Q1 = (arr->at(index - 1) + arr->at(index)) / 2;
			data->q1 = Q1;
		}
		else
		{
			double Q1 = arr->at(median(0, mid_index));
			data->q1 = Q1;
		}

		if (mid_index % 2 == 0)
		{
			unsigned int index = median(mid_index + 1, arr->size());
			double Q3 = (arr->at(index - 1) + arr->at(index)) / 2;
			data->q3 = Q3;
		}
		else
		{
			double Q3 = arr->at(median(mid_index + 1, arr->size()));
			data->q3 = Q3;
		}
		data->iqr = data->q3 - data->q1;
	}

	int count(std::vector<double> *arr, double x)
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

	double mode(std::vector<double> *arr)
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

	oneVarStats getOneVarStats(std::vector<double> *arr)
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

		IQR(arr, &data);

		data.sum = complexSum(arr);
		double mean = data.sum / arr->size();

		double totalDeviation = 0;
		for (double i : *arr)
		{
			totalDeviation += pow((i - mean), 2);
		}

		data.mean = mean;
		data.variance = totalDeviation / (arr->size() - 1);
		data.std = sqrt(data.variance);

		data.mode = mode(arr);

		return data;
	}

	inline double calcZScore(double value, double mean, double std)
	{
		return (value - mean) / std;
	}

	double calcZScore(double value, oneVarStats stats)
	{
		return calcZScore(value, stats.mean, stats.std);
	}

	double calcZScore(double value, std::vector<double> *values)
	{
		oneVarStats data = getOneVarStats(values);

		return calcZScore(value, data.mean, data.std);
	}

	// calculates CDF using error function
	double normalCDF(double zScore)
	{
		// https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
		return 0.5 * std::erfc(-zScore * SQRT_1_2);
	}

	double invNormalCDF(double value)
	{
		// function being implemented
		return 0.0;
	}

	double calcPValue(double value, double mean, double std)
	{
		return 1 - normalCDF((value - mean) / std);
	}

	double calcPValue(double value, oneVarStats stats)
	{
		return calcPValue(value, stats.mean, stats.std);
	}

	double calcPValue(double value, std::vector<double> *values)
	{
		oneVarStats data = getOneVarStats(values);

		return calcPValue(value, data.mean, data.std);
	}

	interval calcInterval(double confidence, double mean, double std);

	const linearRegression calcLinearRegression(std::vector<xyPair> *nums)
	{
		std::vector<double> x;
		std::vector<double> y;
		for (xyPair pair : *nums)
		{
			x.push_back(pair.x);
			y.push_back(pair.y);
		}
		oneVarStats xValues = getOneVarStats(&x);
		oneVarStats yValues = getOneVarStats(&y);

		double sum = 0;
		for (xyPair pair : *nums)
		{
			sum += calcZScore(pair.x, xValues.mean, xValues.std) * calcZScore(pair.y, yValues.mean, yValues.std);
		}

		const linearRegression result(sum / (nums->size() - 1), xValues, yValues);

		return result;
	}

} // namespace stats

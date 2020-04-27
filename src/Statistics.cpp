// Statisitcs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef SQRT_1_2
#define SQRT_1_2 0.7071067812
#endif

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Statisitcs.hpp"
#include "Tables.hpp"

st::oneVarStats st::getOneVarStats(std::vector<double> &arr)
{
	std::sort(arr.begin(), arr.end());

	oneVarStats data;

	data.size = arr.size();
	data.min = arr.front();
	data.max = arr.back();

	unsigned int m_index;

	if (arr.size() % 2 != 0)
	{
		m_index = (arr.size() - 1) / 2;
		data.median = arr.at(m_index);
	}
	else
	{
		data.median = (arr.at(arr.size() / 2 - 1) + arr.at(arr.size() / 2)) / 2;
	}

	st::IQR(arr, data);

	double sum = 0;
	for (double i : arr)
	{
		sum += i;
	}

	data.sum = sum;
	double mean = sum / arr.size();

	double totalDeviation = 0;
	for (double i : arr)
	{
		totalDeviation += pow((i - mean), 2);
	}

	data.mean = mean;
	data.std = sqrt(totalDeviation / (arr.size() - 1));

	return data;
}

unsigned int st::median(unsigned int l, unsigned int r)
{
	int n = r - l + 1;
	n = (n + 1) / 2 - 1;
	return n + l;
}

void st::IQR(std::vector<double> &arr, st::oneVarStats &data)
{
	unsigned int mid_index = st::median(0, arr.size());

	if (mid_index % 2 == 0)
	{
		unsigned int index = st::median(0, mid_index);
		double Q1 = (arr.at(index - 1) + arr.at(index)) / 2;
		data.q1 = Q1;
	}
	else
	{
		double Q1 = arr.at(st::median(0, mid_index));
		data.q1 = Q1;
	}

	if (mid_index % 2 == 0)
	{
		unsigned int index = st::median(mid_index + 1, arr.size());
		double Q3 = (arr.at(index - 1) + arr.at(index)) / 2;
		data.q3 = Q3;
	}
	else
	{
		double Q3 = arr.at(st::median(mid_index + 1, arr.size()));
		data.q3 = Q3;
	}

	data.iqr = data.q3 - data.q1;
}

double st::calcZScore(double value, double mean, double std)
{
	return (value - mean) / std;
}

double st::calcZScore(double value, st::oneVarStats stats)
{
	return st::calcZScore(value, stats.mean, stats.std);
}

double st::calcZScore(double value, std::vector<double> values)
{
	st::oneVarStats stats = st::getOneVarStats(values);
	
	return st::calcZScore(value, stats.mean, stats.std);
} 

double st::normalCDF_calc(double value)
{
	return 0.5 * std::erfc(-value * SQRT_1_2);
}

double st::normalCDF_table(double value)
{
	return st::cdf.at(value);
}

int main()
{

	std::cout << st::normalCDF_calc(2.00) << std::endl;

	return 0;
}
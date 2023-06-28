#pragma once

#include <math.h>
#include <vector>
#include <set>
#include <algorithm>
#include <numbers>
#include <utility>

namespace stats
{
	namespace constants 
	{
		constexpr double SQRT_1_2 = 0.7071067812;
	} // namespace stats::constants

	struct OneVarStats
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

	struct LinearRegression
	{
		double r;
		double rSquared;
		double slope;
		double yIntercept;

		LinearRegression(double r, const OneVarStats &x, const OneVarStats &y)
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

		interval(double min, double mean, double max, double confidence)
			: min(min), mean(mean), max(max), confidence(confidence)
		{
		}
	};

	double simpleSum(const std::vector<double> &arr)
	{
		double sum = 0;
		for (double num : arr)
		{
			sum += num;
		}

		return sum;
	}

	// Improved Kahan–Babuška algorithm
	double complexSum(const std::vector<double> &arr)
	{
		double sum = 0.0;
		double c = 0.0;
		for (double num : arr)
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

	inline double arithmeticMean(const std::vector<double> &arr)
	{
		return complexSum(arr) / arr.size();
	}

	double harmonicMean(const std::vector<double> &arr)
	{
		double sum = 0;
		for (double num : arr)
		{
			sum += 1 / num;
		}

		return arr.size() / sum;
	}

	double geometricMean(const std::vector<double> &arr)
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

	void IQR(const std::vector<double> &arr, OneVarStats &data)
	{
		unsigned int mid_index = median(0, arr.size());

		if (mid_index % 2 == 0)
		{
			unsigned int index = median(0, mid_index);
			double Q1 = (arr.at(index - 1) + arr.at(index)) / 2;
			data.q1 = Q1;
		}
		else
		{
			double Q1 = arr.at(median(0, mid_index));
			data.q1 = Q1;
		}

		if (mid_index % 2 == 0)
		{
			unsigned int index = median(mid_index + 1, arr.size());
			double Q3 = (arr.at(index - 1) + arr.at(index)) / 2;
			data.q3 = Q3;
		}
		else
		{
			double Q3 = arr.at(median(mid_index + 1, arr.size()));
			data.q3 = Q3;
		}
		data.iqr = data.q3 - data.q1;
	}

	double mode(const std::vector<double> &arr)
	{
		std::set<double> s(arr.begin(), arr.end());

		int maxOccurrences = 1;
		double mode = arr.at(0);
		for (double num : s)
		{
			int occurrences = std::count(arr.begin(), arr.end(), num);
			if (occurrences > maxOccurrences)
			{
				maxOccurrences = occurrences;
				mode = num;
			}
		}

		return mode;
	}

	OneVarStats getOneVarStats(std::vector<double> &arr)
	{
		std::sort(arr.begin(), arr.end());

		OneVarStats data;

		data.size = arr.size();
		data.min = arr.front();
		data.max = arr.back();

		size_t m_index;

		if (arr.size() % 2 != 0)
		{
			m_index = (arr.size() - 1) / 2;
			data.median = arr.at(m_index);
		}
		else
		{
			data.median = (arr.at(arr.size() / 2 - 1) + arr.at(arr.size() / 2)) / 2;
		}

		IQR(arr, data);

		data.sum = complexSum(arr);
		double mean = arithmeticMean(arr);

		double totalDeviation = 0;
		for (double i : arr)
		{
			totalDeviation += pow((i - mean), 2);
		}

		data.mean = mean;
		data.variance = totalDeviation / (arr.size() - 1);
		data.std = sqrt(data.variance);

		data.mode = mode(arr);

		return data;
	}

	inline double calcZScore(double value, double mean, double std)
	{
		return (value - mean) / std;
	}

	double calcZScore(double value, OneVarStats stats)
	{
		return calcZScore(value, stats.mean, stats.std);
	}

	double calcZScore(double value, std::vector<double> &values)
	{
		OneVarStats data = getOneVarStats(values);

		return calcZScore(value, data.mean, data.std);
	}

	// calculates CDF using error function
	double normalCDF(double zScore)
	{
		// https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
		return 0.5 * std::erfc(-zScore * constants::SQRT_1_2);
	}

	double inverf(double z)
	{
		return 0.5 * sqrt(std::numbers::pi) * (z + std::numbers::pi / 12 * pow(z, 3) + (7 * pow(std::numbers::pi, 2)) / 480 * pow(z, 5) + (127 * pow(std::numbers::pi, 3)) / 40320 * pow(z, 7) + (4369 * pow(std::numbers::pi, 4)) / 5806080 * pow(z, 9) + (34807 * pow(std::numbers::pi, 5)) / 182476800 * pow(z, 11));
	}

	double invNormalCDF(double value)
	{
		return inverf(1 - 2 * value) / (-1 * constants::SQRT_1_2);
	}

	double calcPValue(double value, double mean, double std)
	{
		return 1 - normalCDF((value - mean) / std);
	}

	double calcPValue(double value, OneVarStats stats)
	{
		return calcPValue(value, stats.mean, stats.std);
	}

	double calcPValue(double value, std::vector<double> &values)
	{
		OneVarStats data = getOneVarStats(values);

		return calcPValue(value, data.mean, data.std);
	}

	interval calcInterval(double confidence, uint32_t n, double p)
	{
		double me = invNormalCDF((1 - confidence) / 2 + confidence) * sqrt(((1 - p) * p) / n);
		return interval(p - me, p, p + me, confidence);
	}

	const LinearRegression calcLinearRegression(const std::vector<std::pair<double, double>> &pairs)
	{
		std::vector<double> x;
		std::vector<double> y;
		for (std::pair<double, double> pair : pairs)
		{
			x.push_back(pair.first);
			y.push_back(pair.second);
		}
		OneVarStats xValues = getOneVarStats(x);
		OneVarStats yValues = getOneVarStats(y);

		double sum = 0;
		for (std::pair<double, double> pair : pairs)
		{
			sum += calcZScore(pair.first, xValues.mean, xValues.std) * calcZScore(pair.second, yValues.mean, yValues.std);
		}

		const LinearRegression result(sum / (pairs.size() - 1), xValues, yValues);

		return result;
	}

} // namespace stats

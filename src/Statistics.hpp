#pragma once

#include <vector>

namespace stats
{

	struct oneVarStats
	{
		double sum;
		double mean;
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

		linearRegression(double r, stats::oneVarStats &x, stats::oneVarStats &y)
			: r(r)
		{
			rSquared = r * r;
			slope = r * y.std / x.std;
			yIntercept = y.mean - slope * x.mean;
		}
	};

	double simpleSum(const std::vector<double> *arr);
	double complexSum(const std::vector<double> *arr);

	oneVarStats getOneVarStats(std::vector<double> *arr);
	unsigned int median(unsigned int l, size_t r);
	void IQR(const std::vector<double> *arr, stats::oneVarStats *data);

	double calcZScore(double value, double mean, double std);
	double calcZScore(double value, oneVarStats stats);
	double calcZScore(double value, std::vector<double> *values);

	// calculates CDF using error function
	double normalCDF(double value);

	double mode(std::vector<double> *arr);
	int count(std::vector<double> *arr, double x);

	const stats::linearRegression calcLinearRegression(std::vector<xyPair> *nums);

} // namespace stats

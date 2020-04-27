#pragma once

#include <vector>

namespace st
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
	};

	oneVarStats getOneVarStats(std::vector<double> &arr);
	unsigned int median(unsigned int l, unsigned int r);
	void IQR(std::vector<double> &arr, st::oneVarStats &data);

}
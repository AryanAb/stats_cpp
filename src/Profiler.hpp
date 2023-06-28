#pragma once

#include <iostream>
#include <chrono>

namespace stats 
{
	//using p_getOneVarStats = stats::oneVarStats(*)(std::vector<double>*);
	//void timer(std::vector<double>* nums, p_getOneVarStats func);

	template<typename F,typename T>
	void timer(F func, T inputs)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func(inputs);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end - start;

		std::cout << duration.count() << std::endl;
	}
}
#pragma once

namespace stats
{

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
# stats_cpp

[![Build Status](https://travis-ci.com/AryanAb/stats_cpp.svg?branch=master)](https://travis-ci.com/AryanAb/stats_cpp)

stats_cpp is a bare-bones header only C++ statistics library. It is designed to be fast and easy to use with the ability to be added to existing projects without much overhead.

## Table of Contents

* [Install](#install)
* [How to Use](#how-to-use)
* [Features](#features)
  * [Sum](#sum)
  * [Arithmetic, Harmonic, and Geometric Mean](#arithmetic-harmonic-and-geometric-mean)
  * [Median and Mode](#median-and-mode)
  * [Variance and Standard Deviation](#variance-and-standard-deviation)
  * [Minimum and Maximum](#minimum-and-maximum)
  * [Q1, Q3, and IQR](#q1-q3-and-iqr)
  * [Z-Score](#z-score)
  * [Normal CDF and Inverse Normal CDF](#normal-cdf-and-inverse-normal-cdf)
  * [P-Value](#p-value)
  * [Confidence Interval](#confidence-interval)
* [Performance](#performance)

## Install

Adding stats_cpp to your project is very simple. Simply add `Statistics.hpp` to your project, and then include it as a header file. You should be able to use the ```stats``` namespace which includes all the functions and structures.

## How to Use

Firstly, note that the data points must be stored in a vector in order to use the functions provided.

Most of the data are stored in special structures. Currently, some types of properties are only available through these structures and cannot be evaluated using function calls. The most important type of this structures is ```oneVarStats```. More details on how to use this and other structures are described below.

## Features

Some properties have multiple ways of calculating. It is recommended to use the function `getOneVarStats()`, provided that the property can be calculated that way. This way, any other future property that might be needed in the future is already calculated and stored in the memory.

Currently, the following properties can be calculated using the library:

### Sum

```c++
std::vector<double> data { ... };

// using normal addition
double sum_1 = stats::simpleSum(&data);

// using Improved Kahan–Babuška algorithm
double sum_2 = stats::complexSum(&data);

// getOneVarStats uses the Improved Kahan–Babuška algorithm
stats::oneVarStats ovs = stats::getOneVarStats(&data);
double sum_3 = ovs.sum;
```

### Arithmetic, Harmonic, and Geometric Mean

#### Arithmetic Mean

```c++
std::vector<double> data { ... };

double a_mean_1 = stats::arithmeticMean(data);

stats::oneVarStats ovs = stats::getOneVarStats(data);
double a_mean_2 = ovs.mean;
```

#### Harmonic Mean

```c++
std::vector<double> data { ... };

double h_mean = stats::harmonicMean(data);
```

#### Geometric Mean

```c++
std::vector<double> data { ... };

double g_mean = geometricMean(data);
```

### Median and Mode

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(data);
double median = ovs.median;
double mode = ovs.mode;
```

### Variance and Standard Deviation

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(data);
double variance = ovs.variance;
double standard_deviation = ovs.std;
```

### Size

```c++
std::vector<double> data { ... };

double size_1 = data.size();

stats::oneVarStats ovs = stats::getOneVarStats(data);
double size_2 = ovs.size;
```

### Minimum and Maximum

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(data);
double min = ovs.min;
double max = ovs.max;
```

### Q1, Q3, and IQR

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(data);
double q1 = ovs.q1;
double q3 = ovs.q3;
double iqr = ovs.iqr;
```

### Z-Score

```c++
std::vector<double> data { ... };
double value = X_VALUE;

double z1 = stats::calcZScore(value, data);

stats::oneVarStats ovs = stats::getOneVarStats(data);
double z2 = stats::calcZScore(value, ovs);

double mean = MEAN_VALUE;
double std = STANDARD_DEVIATION_VALUE;
double z3 = stats::calcZScore(value, mean, std);
```

### Normal CDF and Inverse Normal CDF

```c++
double zScore = SOME_SCORE;
double cdf = stats::normalCDF(zScore);
double newZScore = stats::invNormalCDF(cdf);
```

### P-Value

```c++
std::vector<double> data { ... };
double value = SOME_VALUE;

double p1 = calcPValue(value, data);

stats::oneVarStats ovs = stats::getOneVarStats(data);
double p2 = calcPValue(value, ovs);

double mean = MEAN_VALUE;
double std = STANDARD_DEVIATION_VALUE;
double p3 = calcPValue(value, mean, std);
```

### Confidence Interval

```c++
double confidence = 0.05;
double size = 100;
double parameter = P;

stats::interval ci = stats::calcInterval(confidence, size, parameter);
```

## Performance

This repo contains a very simple profiler in `Profiler.hpp` that could be used for benchmarking. An example is given in `examples/Benchmark.cpp`.

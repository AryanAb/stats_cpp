# stats_cpp

[![Build Status](https://travis-ci.com/AryanAb/stats_cpp.svg?branch=master)](https://travis-ci.com/AryanAb/stats_cpp)

stats_cpp is a simple header only C++ statistics library. It is designed to be fast and easy to use with the ability to
be added to existing projects without much overhead.

## Table of Contents

* [Implementing](#implementing)
* [How to Use](#how-to-use)
* [Features](#features)
  * [Sum](#sum)
  * [Mean](#mean)
  * [Median](#median)
  * [Mode](#mode)
  * [Arithmetic Mean](#arithmetic-mean)
  * [Harmonic Mean](#harmonic-mean)
  * [Geometric Mean](#geometric-mean)
  * [Median](#median)
  * [Mode](#mode)
  * [Variance](#variance)

* [Performance](#performance)

## Implementing

Adding stats_cpp to your project is very simple. Simply add Statistics.hpp to your project, and then include it as a header file. You should be able to use the ```stats``` namespace which includes all the functions and structures.

Note that the data points must be stored in an array in order to use most of the functions provided.

## How to Use

Most of the data are stored in special structures. Currently, some types of properties are only available through these structures and cannot be evaluated using function calls. The most important type of this structures is ```oneVarStats```. More details on how to use this and other structures are described below.

## Features

Note that there are multiple ways of calculating some properties. It is recommended to use the function ```getOneVarStats()```, provided that the property can be calculated that way. This way, any other future property that might be needed in the future is already calculated and stored in the memory.

Currently, the following properties can be calculated using the library:

### Sum

```c++
std::vector<double> data { ... };

// using normal addition
double sum_1 = stats::simpleSum(&data);

// using IImproved Kahan–Babuška algorithm
double sum_2 = stats::complexSum(&data);

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double sum_3 = ovs.sum;


```

### Arithmetic Mean

```c++
std::vector<double> data { ... };

double a_mean_1 = stats::arithmeticMean(*data);

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double a_mean_2 = ovs.mean;
```

### Harmonic Mean

```c++
std::vector<double> data { ... };

double h_mean = stats::harmonicMean(*data);
```

### Geometric Mean

```c++
std::vector<double> data { ... };

double g_mean = geometricMean(*data);
```

### Median and Mode

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double median = ovs.median;
double mode = ovs.mode;
```

### Variance and Standard Deviation

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double variance = ovs.variance;
double standard_deviation = ovs.std;
```

### Size

```c++
std::vector<double> data { ... };

double size_1 = data.size();

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double size_2 = ovs.size;
```

### Minimum and Maximum

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double min = ovs.min;
double max = ovs.max;
```

### Q1, Q3, and IQR

```c++
std::vector<double> data { ... };

stats::oneVarStats ovs = stats::getOneVarStats(&data);
double q1 = ovs.q1;
double q3 = ovs.q3;
double iqr = ovs.iqr;
```

### Z-Score

### Normal CDF

### Inverse Normal CDF

### P-Value

### Confidence Interval

## Performance

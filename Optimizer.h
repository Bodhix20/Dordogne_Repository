#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <functional>
#include <numeric>

using namespace std;

typedef double(*func)(vector<double>); ///< Pointer to a function that takes an array of doubles representing a value in each dimension and returns a double.

class Optimizer {

protected :

	func objectiveFunction;

public:

	//Basic Constructor
	Optimizer();

	//Constructor with the fitness function
	Optimizer(func objectiveFunction);

	//The different optimization schemes
	vector<double> pso(int dim, int swarm_size, int max_iter, double lower_bound, double upper_bound);
	vector<double> nelder_mead(int dim, int max_iter, double tol, double lower_bound, double upper_bound);
	vector<double> dfp(const std::vector<double>& x0, int max_iter = 1000, double tol = 1e-8);

};

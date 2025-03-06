#include "Tower.h"
#include "csvCreator.h"
#include "Optimizer.h"
#include <chrono>
#include <fstream>

// declaring functions that ill appear later
double TowerObjectiveFunction(vector<double> x);

//declaring objects
Tower testTower;
Optimizer optimizer(TowerObjectiveFunction); 
CSVCreator writer("optimalTower.csv");

//double R0 = 39.3;
//double Rm = 27.4;

//Creating the frustum heights array
int N = 4; //the number of points
double maxHeight = 50;
double dy = maxHeight / (N - 1);
vector<double> frustumHeights;


int main() {	
	vector<double> xvalues, yvalues, zvalues;
	double lowerBound = 0;
	double upperBound = maxHeight;
	int resolution = 200; // Adjust for finer or coarser resolution

	//Initializing the frustum heights array
	for (int i = 0; i < N; i++) {
		frustumHeights.push_back(i * dy);
	}

	for (double x1 = lowerBound; x1 <= upperBound; x1 += (upperBound - lowerBound) / resolution) {
		for (double x2 = lowerBound; x2 <= upperBound; x2 += (upperBound - lowerBound) / resolution) {
			vector<double> x = { x1, x2 }; // Two variable radii
			double fValue = TowerObjectiveFunction(x);

			xvalues.push_back(x1);
			yvalues.push_back(x2);
			zvalues.push_back(fValue);
		}
	}

	ofstream file("objective_function_data.csv");
	file << "x1,x2,z\n";
	for (size_t i = 0; i < xvalues.size(); i++) {
		file << xvalues[i] << "," << yvalues[i] << "," << zvalues[i] << "\n";
	}
	file.close();

}

//objective function to maximize
double TowerObjectiveFunction(vector<double> x){

	//fixing the optimization parameters
	vector<double> sectionRadii = { 39.3, 28, 25, 27.4 };
	vector<double> sectionHeight(sectionRadii.size());
	double fixedVolume = 130000;
	double alpha = 1; //volume
	double beta = 5000; //height penalty
	double objective_std_deviation = 1;



	if (x.size() < sectionRadii.size() - 2) {
		cerr << "Please input a vector of" << sectionRadii.size() - 2 << " points \n";
		return 0;
	}

	sectionHeight[0] = 0;
	for (int i = 1; i < sectionHeight.size() - 1; i++) {
		sectionHeight[i] = x[i - 1];
	}
	sectionHeight[sectionHeight.size() - 1] = maxHeight;

	//Adding a penalty on the heights of sections
	double heightPenalty = 0.0;
	for (int i = 0; i < sectionHeight.size() - 1; i++) {
		if (sectionHeight[i] >= sectionHeight[i + 1]) heightPenalty += pow(1 + sectionHeight[i] - sectionHeight[i + 1], 2);
	}



	//calculating the standard deviation
	/*
	double sum = 0.0;
	for (int i = 0; i < sectionHeight.size(); i++) {
		sum += sectionHeight[i];
	}
	double mean = sum / sectionHeight.size();
	sum = 0.0;
	for (int i = 0; i < sectionHeight.size(); i++) {
		sum += pow(mean - sectionHeight[i], 2);
	}
	double std_deviation = sqrt(sum / ((sectionHeight.size() - 1)));

	*/


	//Calculating the taper to the middle
	/*
	double curvePenalty = 0.0;
	for (int i = 0 ; i < sectionHeight.size()-1; i++) {

		if (i <= sectionHeight.size() / 2)
		{
			if (sectionHeight[i] < sectionHeight[i + 1]) {
				curvePenalty += sigma * pow(sectionHeight[i] - sectionHeight[i + 1],2);
			}
		}
		else if(i > sectionHeight.size() / 2) {
			if (sectionHeight[i] > sectionHeight[i + 1]) {
				curvePenalty += sigma * pow(sectionHeight[i] - sectionHeight[i + 1], 2);
			}
		}
	}
	*/


	testTower.setFrustumHeigths(sectionHeight);
	testTower.setSectionRadii(sectionRadii);
	double totalArea = testTower.calculateTotalArea();
	double totalVolume = testTower.calculateTotalVolume();

	//cout << "Total area " << totalArea << "m^2 \n";
	//cout << "Total volume " << totalVolume << "m^3 \n";

	//+ beta * pow(std_deviation - objective_std_deviation,2)

	//FITNESS FUNCTION-------------------------------------------------------------------------------------------------------------------------------------
	double fitnessValue = totalArea + alpha * pow(totalVolume - fixedVolume, 2) + beta * heightPenalty;
	//-----------------------------------------------------------------------------------------------------------------------------------------------------

	cout << "Fitness value : " << fitnessValue << "\n ";

	return fitnessValue;

}
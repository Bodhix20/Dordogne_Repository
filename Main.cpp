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

double R0 = 39.3;
double Rm = 27.4;

//Creating the frustum heights array
int N = 4; //the number of points
double maxHeight = 50;
double dy = maxHeight / (N - 1);
vector<double> frustumHeights;


int main() {	
	vector<double> xvalues, yvalues, zvalues;
	double lowerBound = 20;
	double upperBound = 40;
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
	//vector<double> frustumHeigths = { 0.0, 3.6, 7.3, 10.9, 14.6, 18.2, 21.9, 25.5, 29.1, 32.8, 36.5 };
	vector<double> sectionRadii(frustumHeights.size());
	double fixedVolume = 130000;
	double alpha = 1; //volume
	double beta = 50; //std deviation
	double sigma = 1000; //curve control
	double objective_std_deviation = 1;


	if (x.size() < frustumHeights.size()-2) {
		cerr << "Please input a vector of" << frustumHeights.size()-2 <<  " points \n";
		return 0;
	}


	sectionRadii[0] = R0;
	for (int i = 1; i < sectionRadii.size() - 1; i++) {
		sectionRadii[i] = x[i - 1];
	}
	sectionRadii[sectionRadii.size() -1] = Rm;

	//calculating the standard deviation
	double sum = 0.0;
	for (int i = 0; i < sectionRadii.size(); i++) {
		sum += sectionRadii[i];
	}
	double mean = sum / sectionRadii.size();
	sum = 0.0;
	for (int i = 0; i < sectionRadii.size(); i++) {
		sum += pow(mean - sectionRadii[i], 2);
	}
	double std_deviation = sqrt(sum / ((sectionRadii.size() - 1)));


	//Calculating the taper to the middle
	double curvePenalty = 0.0;
	for (int i = 0 ; i < sectionRadii.size()-1; i++) {

		if (i <= sectionRadii.size() / 2)
		{
			if (sectionRadii[i] < sectionRadii[i + 1]) {
				curvePenalty += sigma * pow(sectionRadii[i] - sectionRadii[i + 1],2);
			}
		}
		else if(i > sectionRadii.size() / 2) {
			if (sectionRadii[i] > sectionRadii[i + 1]) {
				curvePenalty += sigma * pow(sectionRadii[i] - sectionRadii[i + 1], 2);
			}
		}
	}


	testTower.setSectionRadii(sectionRadii);
	testTower.setFrustumHeigths(frustumHeights);
	double totalArea = testTower.calculateTotalArea();
	double totalVolume = testTower.calculateTotalVolume();

	//cout << "Total area " << totalArea << "m^2 \n";
	//cout << "Total volume " << totalVolume << "m^3 \n";

	//+ beta * pow(std_deviation - objective_std_deviation,2)

	//FITNESS FUNCTION-------------------------------------------------------------------------------------------------------------------------------------
	double fitnessValue = totalArea + alpha * pow(totalVolume - fixedVolume, 2) + curvePenalty + beta * pow(std_deviation - objective_std_deviation, 2);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	
	cout << "Fitness value : " << fitnessValue << "\n ";

	return fitnessValue ;

}
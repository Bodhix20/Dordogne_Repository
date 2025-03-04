#include "Tower.h"
#include "csvCreator.h"
#include "Optimizer.h"
#include <chrono>

// declaring functions that ill appear later
double TowerObjectiveFunction(vector<double> x);

//declaring objects
Tower testTower;
Optimizer optimizer(TowerObjectiveFunction); 
CSVCreator writer("optimalTower.csv");

//double R0 = 39.3;
//double Rm = 27.4;

//Creating the frustum heights array
double maxHeight = 50;



int main() {	

	//Optimization variables
	double dimension = 9;
	double swarm_size = 1000;
	double max_iter = 2000;
	double lower_bound = 0;
	double uper_bound = maxHeight;

	//Launching the optimizer 
	auto start = std::chrono::high_resolution_clock::now(); //starting the clock

	vector<double> optimalHeights = optimizer.pso(dimension,swarm_size,max_iter,lower_bound,uper_bound); //Getting the optimal results
	
	auto stop = std::chrono::high_resolution_clock::now();// Stop measuring time

	optimalHeights.insert(optimalHeights.begin(), 0); //Adding 0 and maxHeight
	optimalHeights.push_back(maxHeight);
	vector<double> Radii = testTower.getSectionRadii();
	
	auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);// Calculate duration

	vector<double> Frustums = testTower.getFrustumHeights();

	writer.generateCSV(Radii, optimalHeights); //Generating the csv file

	//Displaying the analysis data
	cout << "\nAnalysis data : \n";
	cout << "Calculation time: " << durationMs.count() << " ms" << "\n";
	cout << "Dimension : " << dimension << "\n" << "Swarm_size : " << swarm_size << "\n" << "Max iterations : " << max_iter << "\n"
		<< "Bounds : " << "[" << lower_bound << " ; " << uper_bound << "] \n";

	//Displaying the optimal solution
	cout << "\n" << "Optimal solution : \n";

	for (int i = 0; i < Radii.size(); i++) {
		cout << "R" << i << " : " << Radii[i] << "\n";
	}
	cout << "\n";

	for (int i = 0; i < optimalHeights.size(); i++) {
		cout << "h" << i << " : " << optimalHeights[i] << "\n";
	}
	cout << "\n";

	testTower.setSectionRadii(Radii);
	testTower.setFrustumHeigths(optimalHeights);
	
	cout << "Total area : " << testTower.calculateTotalArea() << "m^2 \n";
	cout << "Total volume : " << testTower.calculateTotalVolume() << "m^3 \n";

	//calculating the standard deviation
	double sum = 0.0;
	for (int i = 0; i < Radii.size(); i++) {
		sum += Radii[i];
	}
	double mean = sum / Radii.size();
	sum = 0.0;
	for (int i = 0; i < Radii.size(); i++) {
		sum += pow(mean - Radii[i], 2);
	}
	double std_deviation = sqrt(sum / ((Radii.size() - 1)));
	cout << "Standard deviation : " << std_deviation << "\n";
}

//objective function to maximize
double TowerObjectiveFunction(vector<double> x){

	//fixing the optimization parameters
	vector<double> sectionRadii = { 39.3, 35, 30, 29, 28, 27, 24, 24, 23, 25, 27.4 };
	vector<double> sectionHeight(sectionRadii.size());
	double fixedVolume = 130000;
	double alpha = 1; //volume
	double beta = 5000; //height penalty
	double objective_std_deviation = 1;



	if (x.size() < sectionRadii.size()-2) {
		cerr << "Please input a vector of" << sectionRadii.size()-2 <<  " points \n";
		return 0;
	}

	sectionHeight[0] = 0;
	for (int i = 1; i < sectionHeight.size() - 1; i++) {
		sectionHeight[i] = x[i - 1];
	}
	sectionHeight[sectionHeight.size() -1] = maxHeight;

	//Adding a penalty on the heights of sections
	double heightPenalty = 0.0;
	for (int i = 0; i < sectionHeight.size()-1; i++) {
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
	
	//cout << "Fitness value : " << fitnessValue << "\n ";

	return fitnessValue ;

}
#include "Tower.h"
#include "csvCreator.h"
#include "Optimizer.h"

// declaring functions that ill appear later
double TowerObjectiveFunction(vector<double> x);

//declaring objects
Tower testTower;
Optimizer optimizer(TowerObjectiveFunction); 
CSVCreator writer("optimalTower.csv");

double R0 = 39.3;
double Rm = 27.4;

int main() {	

	//Launching the optimizer 
	vector<double> optimalRadii = optimizer.pso(9,1000,1000,20,40); //Getting the optimal results
	optimalRadii.insert(optimalRadii.begin(), R0); //Adding R0 and Rm
	optimalRadii.push_back(Rm);
	vector<double> Frustums = testTower.getFrustumHeights();
	writer.generateCSV(optimalRadii, Frustums); //Generating the csv file

	//Displaying the optimal solution
	cout << "\n\n" << "Optimal solution : \n";

	for (int i = 0; i < optimalRadii.size(); i++) {
		cout << "R" << i << " : " << optimalRadii[i] << "\n";
	}
	cout << "\n";

	for (int i = 0; i < Frustums.size(); i++) {
		cout << "h" << i << " : " << Frustums[i] << "\n";
	}
	cout << "\n";

	testTower.setSectionRadii(optimalRadii);
	testTower.setFrustumHeigths(Frustums);
	
	cout << "Total area : " << testTower.calculateTotalArea() << "m^2 \n";
	cout << "Total volume : " << testTower.calculateTotalVolume() << "m^3 \n";

	//calculating the standard deviation
	double sum = 0.0;
	for (int i = 0; i < optimalRadii.size(); i++) {
		sum += optimalRadii[i];
	}
	double mean = sum / optimalRadii.size();
	sum = 0.0;
	for (int i = 0; i < optimalRadii.size(); i++) {
		sum += pow(mean - optimalRadii[i], 2);
	}
	double std_deviation = sqrt(sum / ((optimalRadii.size() - 1)));
	cout << "Standard deviation : " << std_deviation << "\n";
}

//objective function to maximize
double TowerObjectiveFunction(vector<double> x){

	//fixing the optimization parameters
	vector<double> frustumHeigths = { 0.0, 3.6, 7.3, 10.9, 14.6, 18.2, 21.9, 25.5, 29.1, 32.8, 36.5 };
	vector<double> sectionRadii(frustumHeigths.size());
	double fixedVolume = 130000;
	double alpha = 1;
	double beta = 50;
	double sigma = 1000;
	double objective_std_deviation = 3;



	if (x.size() < frustumHeigths.size()-2) {
		cerr << "Please input a vector of" << frustumHeigths.size()-2 <<  " points \n";
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
		else if(i >= sectionRadii.size() / 2) {
			if (sectionRadii[i] > sectionRadii[i + 1]) {
				curvePenalty += sigma * pow(sectionRadii[i] - sectionRadii[i + 1], 2);
			}
		}
	}


	testTower.setSectionRadii(sectionRadii);
	testTower.setFrustumHeigths(frustumHeigths);
	double totalArea = testTower.calculateTotalArea();
	double totalVolume = testTower.calculateTotalVolume();

	//cout << "Total area " << totalArea << "m^2 \n";
	//cout << "Total volume " << totalVolume << "m^3 \n";

	//+ beta * pow(std_deviation - objective_std_deviation,2)

	//FITNESS FUNCTION-------------------------------------------------------------------------------------------------------------------------------------
	double fitnessValue = totalArea + alpha * pow(totalVolume - fixedVolume, 2) + curvePenalty + beta * pow(std_deviation - objective_std_deviation, 2);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	
	//cout << "Fitness value : " << fitnessValue << "\n ";

	return fitnessValue ;

}
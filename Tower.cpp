#include "Tower.h"

//Basic constructor
Tower::Tower() {
	this->sectionRadii = { 10,10,10,10,10,10,10,10,10,10,10 };
	this->frustumHeigths = { 0.0, 3.6, 7.3, 10.9,14.6, 18.2, 21.9, 25.5, 29.1, 32.8 , 36.5 };
}

//Constructor with entry parameters
Tower::Tower(vector<double> sectionRadii, vector<double> frustumHeigths){
	this->sectionRadii = sectionRadii;
	this->frustumHeigths = frustumHeigths;
}

//Method to calculate the area of a specific section of the tower
double Tower::calculateSectionArea(int sectionIndex) {

	//Getting the upper and lower ring radii
	double Ri = sectionRadii[sectionIndex];
	double Rin = sectionRadii[sectionIndex + 1];

	//calculating the height of the current segment
	double di = fabs(frustumHeigths[sectionIndex + 1] - frustumHeigths[sectionIndex]);

	//calculating the angle between the horizontal plane
	double ai = atan(di / (fabs(Ri - Rin)));

	//calculating the height of the cone
	double h = tan(ai) * Ri;

	//calculating the hypotenus of the cone
	double l1 = sqrt(pow(h, 2) + pow(Ri, 2));


	//if the next section is larger than the current one then the tip of the cone flips side and is on Ri instead of Rin
	double a1, a2, l2;
	if(Ri == Rin){
		//cout << "Area at section " << sectionIndex << ": " << M_PI * Ri * di << "m^2 \n";
		return 2*M_PI * Ri * di;
	}
	else if (Ri > Rin) {
		//calculating the total area of the cone
		a1 = M_PI * Ri * l1;

		//calculating the hypotenus of the top section of the cone
		l2 = sqrt(pow((h - di), 2) + pow(Rin, 2));

		//calculating the top section area of the cone
		a2 = M_PI * Rin * l2;
	}
	else {
		//calculating the total area of the cone
		a1 = M_PI * Rin * l1;

		//calculating the hypotenus of the top section of the cone
		l2 = sqrt(pow((h - di), 2) + pow(Ri, 2));

		//calculating the top section area of the cone
		a2 = M_PI * Ri * l2;
	}

	//cout << "Area at section " << sectionIndex << ": " << a1 - a2 << "m^2 \n";

	//calculating the area of the section by substracting the total by the tip
	return a1 - a2;

}

//Method to calculate the total area of the tower
double Tower::calculateTotalArea() {
	double totalArea = 0.0;
	for (int i = 0; i < this->sectionRadii.size()-1; i++) {
		totalArea += this->calculateSectionArea(i);
		//cout << "totalArea in section " << i << " : " << totalArea << "\n";
	}
	return totalArea;
}

double Tower::calculateSectionVolume(int sectionIndex){

	//Getting the upper and lower ring radii
	double Ri = sectionRadii[sectionIndex];
	double Rin = sectionRadii[sectionIndex + 1];

	//calculating the height of the current segment
	double di = fabs(frustumHeigths[sectionIndex + 1] - frustumHeigths[sectionIndex]);

	//calculating the angle between the horizontal plane
	double ai = atan(di / (fabs(Ri - Rin)));

	//calculating the height of the cone
	double h = tan(ai) * Ri;

	//if the next section is larger than the current one then the tip of the cone flips side and is on Ri instead of Rin
	double v1, v2;
	if (Ri == Rin) {
		//cout << "Volume at section " << sectionIndex << ": " << M_PI * pow(Ri, 2) * di << "m^3 \n";
		return M_PI * pow(Ri,2) * di;
	}
	else if (Ri > Rin) {
		//calculating the total volume of the cone
		v1 = (M_PI * pow(Ri, 2) * h) / 3;

		//calculating the top section volume of the cone
		v2 = (M_PI * pow(Rin, 2) * (h-di)) / 3;
	}
	else {
		//calculating the total volume of the cone
		v1 = (M_PI * pow(Rin, 2) * h) / 3;

		//calculating the top section volume of the cone
		v2 = (M_PI * pow(Ri, 2) * (h - di)) / 3;
	}

	//cout << "Volume at section " << sectionIndex << ": " << v1 - v2 << "m^3 \n";

	return v1 - v2;
}

//Method to calculate the total volume of the tower
double Tower::calculateTotalVolume() {
	double totalVolume = 0.0;
	for (int i = 0; i < this->sectionRadii.size()-1; i++) {
		totalVolume += this->calculateSectionVolume(i);
		//cout << "totalVolume in section " << i << " : " << totalVolume << "\n";
	}
	return totalVolume;
}

//Setter for the section radii
void Tower::setSectionRadii(vector<double> sectionRadii) {
	this->sectionRadii = sectionRadii;
}

//Setter for the fulcrum heigths
void Tower::setFrustumHeigths(vector<double> frustumHeigths) {
	this->frustumHeigths = frustumHeigths;
}

//Getter for the section radii vector
vector<double> Tower::getSectionRadii() {
	return this->sectionRadii;
}

//Getter for the frustum heigths vector
vector<double> Tower::getFrustumHeights() {
	return this->frustumHeigths;
}

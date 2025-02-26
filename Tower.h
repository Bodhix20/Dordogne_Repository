#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>

using namespace std;

class Tower {

protected:
	vector<double> sectionRadii;
	vector<double> frustumHeigths;

public:
	//basic constructor
	Tower();
	//constructor with sectionRadii and fulcrum heigths
	Tower(vector<double> sectionRadii, vector<double> frustumHeigths);
	double calculateSectionArea(int sectionIndex);
	double calculateSectionVolume(int sectionIndex);
	double calculateTotalArea();
	double calculateTotalVolume();
	void setSectionRadii(vector<double> sectionRadii);
	void setFrustumHeigths(vector<double> frustumHeights);
	vector<double> getSectionRadii();
	vector<double> getFrustumHeights();
};

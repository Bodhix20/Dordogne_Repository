#include "CSVCreator.h"
#include <fstream>
#include <iostream>

CSVCreator::CSVCreator(const std::string& filename) : filename(filename) {}

void CSVCreator::generateCSV(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        std::cerr << "Error: x and y coordinate arrays must have the same size!" << std::endl;
        return;
    }

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Write header
    file << "StartLoft\n";
    file << "StartCurve\n";

    // Write coordinate data (x, y, 0), multiplied by 1000 to get the units in meters
    for (size_t i = 0; i < x.size(); ++i) {
        file << "0\t" << x[i]*1000 << "\t" << y[i]*1000 << "\n";
    }

    // Write footer
    file << "EndCurve\n";
    file << "EndLoft\n";
    file << "End\n";

    file.close();

    std::cout << "Succesfully created file at " << filename << "\n";
}

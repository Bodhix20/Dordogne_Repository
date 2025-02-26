#pragma once

#ifndef CSV_CREATOR_H
#define CSV_CREATOR_H

#include <string>
#include <vector>

class CSVCreator {
public:
    // Constructor
    CSVCreator(const std::string& filename);

    // Method to generate the CSV file
    void generateCSV(const std::vector<double>& x, const std::vector<double>& y);

private:
    std::string filename;
};

#endif // CSV_CREATOR_H

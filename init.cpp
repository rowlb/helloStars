#include <iostream> // input output stream
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector> // dynamic array
#include <string>

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;

    // open file
    std::ifstream file(filename);

    // error if file open fails
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }


    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }



    file.close();
    return data;
}

int init() {
    auto data = readCSV("stars.csv");
    
    int raicrsCol;
    int deicrsCol;
    int vMagCol;

    // identifying columns (needed in case data moves around)
    int i = 0;
    for (const auto& cell : data.at(0)) {
        if (cell == "RAICRS") {
            raicrsCol = i;
        }
        else if (cell == "DEICRS") {
            deicrsCol = i;
        }
        else if (cell == "Vmag") {
            vMagCol = i;
        }
        i++;
    }
    std::cout << raicrsCol << "\t";
    std::cout << deicrsCol << "\t";
    std::cout << vMagCol << "\t";

    // for (const auto& row : data) {
    //     int j = 0;
    //     for (const auto& cell : row) {
    //         //std::cout << cell << "\t";
    //     }
    //     //std::cout << std::endl;
    //     j++;
    // }

    return 0;
}


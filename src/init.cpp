#include <iostream> // input output stream
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector> // dynamic array
#include <string>
#include "types.h"

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

InitResult init(const std::string& filename) {
    auto data = readCSV(filename);
    
    int raicrsCol;
    int deicrsCol;
    int vMagCol;
    int HIPCol = 0;

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
        // this doesnt work for some reason so i just manually set it
        // else if (cell == "HIP") {
        //     HIPCol = i;
        // }
        i++;
    }

    std::cout << raicrsCol << "\t";
    std::cout << deicrsCol << "\t";
    std::cout << vMagCol << "\t";

    
  
    // list of DNs in DA degree of RA degree
    std::vector<std::vector<std::vector<DegreeNode>>> starLocs(361, std::vector<std::vector<DegreeNode>>(181));
    std::vector<Star> stars;

    
    
    

    for (const auto& row : data) {
        //std::cout << row.at(0) << std::endl;
        int j = 0;
        double raicrs;
        double deicrs;
        double vMag;
        int HIP;
        try {
            for (const auto& cell : row) {
                if (j == raicrsCol) {
                    try { 
                        raicrs = std::stod(cell);
                    }
                    catch (const std::invalid_argument&) { 
                        std::cerr << "Invalid argument for RAICRS stod" << std::endl;
                        break;
                    }
                    catch (const std::out_of_range&) {
                        std::cerr << "stod out of range" << std::endl;
                        break;
                    }
                }
                else if (j == deicrsCol) {
                    try { 
                        deicrs = std::stod(cell);
                    }
                    catch (const std::invalid_argument&) { 
                        std::cerr << "Invalid argument for DEICRS stod" << std::endl;
                        break;
                    }
                    catch (const std::out_of_range&) {
                        std::cerr << "stod out of range" << std::endl;
                        break;
                    }
                }
                else if (j == vMagCol) {
                    try {    
                        vMag = std::stod(cell);
                    }
                    catch (const std::invalid_argument&) { 
                        std::cerr << "Invalid argument for vMag stod" << std::endl;
                        break;
                    }
                    catch (const std::out_of_range&) {
                        std::cerr << "stod out of range" << std::endl;
                        break;
                    }
                }
                else if (j == HIPCol) {
                    try { 
                        HIP = std::stoi(cell);
                    }
                    catch (const std::invalid_argument&) { 
                        std::cerr << "Invalid argument for HIP stoi" << std::endl;
                        break;
                    }
                    catch (const std::out_of_range&) {
                        std::cerr << "stoi out of range" << std::endl;
                        break;
                    }
                }
                j++;
                //std::cout << cell << "\t";
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl << "Something went wrong with this row" << std::endl;
        }
        // populate lists with row info
        try {

            //std::cout << "DE: " << deicrs << std::endl;
            //std::cout << "RA: " << raicrs << std::endl;

            int DEListIndex = static_cast<int>(deicrs + 90); // offset 90 to account for -90 to 90 range
            int RAListIndex = static_cast<int>(raicrs);
            
            int id = stars.size();

            DegreeNode degNode = {id, HIP, raicrs, deicrs};


            //std::cout << RAListIndex << std::endl;
            //std::cout << DEListIndex << std::endl;
            
            starLocs.at(RAListIndex).at(DEListIndex).push_back(degNode);


            Star star = {id, HIP, vMag};

            

            stars.push_back(star);
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl << "Invalid row entry" << std::endl;
            continue;
        }
        
    }
    InitResult result = {stars, starLocs};
    return result;
};

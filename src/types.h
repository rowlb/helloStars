#include <vector>
#pragma once

struct DegreeNode {
    int id;
    int HIP;
    double RAdegree;
    double DEdegree;
};

struct Star {
    int id;
    int HIP;
    double vMag;
};

struct InitResult {   
    std::vector<Star> stars;
    std::vector<std::vector<std::vector<DegreeNode>>> starLocs;
};
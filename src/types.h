#include <vector>
#pragma once

struct DegreeNode {
    int starID;
    double RAdegree;
    double DEdegree;
};

struct Star {
    int starID;
    double vMag;
};

struct InitResult {   
    std::vector<Star> stars;
    std::vector<std::vector<std::vector<DegreeNode>>> starLocs;
};
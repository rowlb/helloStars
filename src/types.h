#include <vector>
#pragma once

struct DegreeNode {
    double degree;
    int starID;
};

struct Star {
    int starID;
    double vMag;
};

struct InitResult {
    std::vector<std::vector<DegreeNode>> degreeListsRA;
    std::vector<std::vector<DegreeNode>> degreesListsDE;    // stored from -90 to 90
    std::vector<Star> stars;
};
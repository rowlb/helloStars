#include "init.h"
#include "types.h"
#include <vector>
#include <iostream>

int main() {
    InitResult lists = init("stars.csv");
    int fullCount = 0;
    for (std::vector<DegreeNode> RAlist : lists.degreeListsRA) {
        fullCount += RAlist.size();
    }
    std::cout << fullCount << std::endl;
}
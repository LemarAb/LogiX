#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "../include/cnf.hpp"

void chooseINC() {
    int i = 0;
    while (vars[curVar].val != FREE) {
        curVar++;
        // std::cout << "Curvar value " << curVar << "\n";
    }

    vars[curVar].val = TRUE;
    vars[curVar].forced = false;
    assig.push(curVar);
    updateCNF(curVar);
    // std::cout << "CHOOSE_LIT:" << curVar << "\n";
}
/*--------------------END FOR CHOOSE INC-----------------------*/

// Custom utility function that helps keep the DLISOccurance set ordered
void chooseDLIS() {
    printf("HI %i", 1);

    int max = 0;
    int index = 0;
    bool pol = false;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].val != FREE) continue;

        int indPosSum = vars[i].pos_occ.size();
        int indNegSum = vars[i].neg_occ.size();

        max = (indPosSum > indNegSum) ? indPosSum : indNegSum;
        index = i;
        pol = (indPosSum > indNegSum);
    }

    curVar = index;
    vars[curVar].val = (pol) ? TRUE : FALSE;
    vars[curVar].forced = false;
    assig.push(curVar);
}

void chooseDLCS() {
    printf("HI %i", 2);

    int max = 0;
    int index = 0;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].val != FREE) continue;

        int totalOcc = vars[i].neg_occ.size() + vars[i].pos_occ.size();
        if (totalOcc > max) {
            max = totalOcc;
            index = i;
        }
    }

    curVar = index;
    if (vars[curVar].pos_occ.size() > vars[curVar].neg_occ.size()) {
        vars[curVar].val = TRUE;
    } else {
        vars[curVar].val = FALSE;
    }
    // std::cout << "CHOOSE_LIT:" << curVar << "\n";
    vars[curVar].forced = false;
    assig.push(curVar);
}
/*---------------------------------END OF DLCS-------------------------------*/

void chooseMOM() {
    printf("HI %i", 3);

    // std::vector<double> MOMScore(numOfClauses, 0.0);
    // int param;

    // for (const auto clause : minimalClauses) {
    //     for (int i = 0; i < cnf[clause].literals.size(); i++) {
    //         auto it = maxHeap.find(std::abs(cnf[clause].literals[i]));
    //         (cnf[clause].literals[i] > 0) ? vars[*it].posCount++ : vars[*it].negCount++;
    //         if (it != maxHeap.end()) maxHeap.erase(std::abs(cnf[clause].literals[i]));
    //         maxHeap.insert(std::abs(cnf[clause].literals[i]));
    //     }
    // }

    // curVar = *(maxHeap.begin());
    // vars[curVar].posCount > vars[curVar].negCount ? vars[curVar].val = TRUE : vars[curVar].val = FALSE;
    // vars[curVar].forced = false;
    // assig.push(curVar);
}

void chooseJW() { printf("HI %i", 4); }
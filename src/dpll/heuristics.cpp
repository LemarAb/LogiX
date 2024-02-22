#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "../../include/cnf.hpp"

void chooseINC() {
    dc++;
    while (vars[curVar].val != FREE) curVar++;

    vars[curVar].val = TRUE;
    vars[curVar].forced = false;
    assig.push(curVar);
    update(curVar);
}

void chooseDLIS() {
    dc++;
    int max = 0;
    int index = 0;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].val != FREE) continue;

        int cur = std::max(vars[i].pos_occ.size(), vars[i].neg_occ.size());
        if (cur > max) {
            max = cur;
            index = i;
        }
    }

    curVar = index;
    vars[curVar].val = vars[curVar].pos_occ.size() > vars[curVar].neg_occ.size() ? TRUE : FALSE;
    vars[curVar].forced = false;
    assig.push(curVar);
    update(curVar);
}

void chooseDLCS() {
    dc++;
    int max = 0;
    int index = 0;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].val != FREE) continue;

        int combinedSum = vars[i].neg_occ.size() + vars[i].pos_occ.size();
        if (combinedSum > max) {
            max = combinedSum;
            index = i;
        }
    }

    curVar = index;
    vars[curVar].val = vars[curVar].pos_occ.size() > vars[curVar].neg_occ.size() ? TRUE : FALSE;
    vars[curVar].forced = false;
    assig.push(curVar);
    update(curVar);
}

void chooseJW() {
    dc++;
    double max = 0;
    int index = 0;
    bool pol = false;

    for (int i = 1; i <= numOfVars; i++) {
        if (vars[i].val != FREE) continue;
        double pos = 0;
        double neg = 0;
        for (int j = 0; j < vars[i].pos_occ.size(); j++) {
            pos += pow(2, -clauses[j].active);
        }
        for (int j = 0; j < vars[i].neg_occ.size(); j++) {
            neg += pow(2, -clauses[j].active);
        }
        double cur = (pos >= neg) ? pos : neg;

        if (cur > max) {
            max = cur;
            index = i;
            pol = (pos >= neg) ? true : false;
        }
    }

    curVar = index;
    vars[curVar].val = pol ? TRUE : FALSE;
    vars[curVar].forced = false;
    assig.push(curVar);
    update(curVar);
}
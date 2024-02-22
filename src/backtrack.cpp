#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "../../include/cnf.hpp"

void backtrack() {
    while (!assig.empty() && vars[assig.top()].forced) {  // until the last branching variable.
        int toUnassign = assig.top();
        vars[toUnassign].setValue(FREE);
        vars[toUnassign].forced = false;
        assig.pop();
        //  std::cout << "Removed literal " << toUnassign << " from assig stack \n";
    }

    // clear unit queue
    while (!unitQueue.empty()) {
        // std::cout << "Element to be popped from queue: " << unitQueue.front() << "\n";
        vars[std::abs(unitQueue.front())].enqueued = false;
        unitQueue.pop();
    }

    if (assig.empty()) {
        pthread_exit((void *)1);
    }  // UNSAT

    // Most recent branching variable
    int b = assig.top();
    int oldval = vars[b].getValue();
    // Assign negated val
    vars[b].forced = true;
    vars[b].setValue(Assig(int(2 - std::pow(2.0, vars[b].getValue()))));
    // std::cout << "New branch var" << b << ", OLD: " << oldval << ", NEW: " << vars[b].getValue();
    curVar = b;
    updateWatchedLiterals(b);
    unitPropagate();
}
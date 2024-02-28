#include "../../include/cdcl.hpp"

void* dpll(void* arg) {  // TODO: We should implement the more optimised appproach of checking the satisfaction of every clause
    while (true) {
        unitPropagate();
        chooseLiteral();
    }
}

void unitPropagate() {
    int unitLiteral;
    while (!unitQueue.empty()) {
        unitLiteral = unitQueue.front();
        //  std::cout << "current queue elm = " << unitLiteral << "\n";
        unitQueue.pop();
        vars[std::abs(unitLiteral)].enqueued = false;
        vars[std::abs(unitLiteral)].forced = true;
        (unitLiteral > 0) ? vars[std::abs(unitLiteral)].setValue(TRUE) : vars[std::abs(unitLiteral)].setValue(FALSE);
        //  std::cout << "UP variable " << unitLiteral << " set to " << vars[std::abs(unitLiteral)].getValue() << "\n";

        assig.push(std::abs(unitLiteral));

        updateWatchedLiterals(std::abs(unitLiteral));
    }
}

void chooseLiteral() {
    switch (heuristic) {
        case INC:
            chooseINC();
            break;
        case DLIS:
            chooseDLIS();
            break;
        case DLCS:
            chooseDLCS();
            break;
        case MOM:
            chooseMOM();
            break;
        case JW:
            chooseJW();
            break;
    }
    updateWatchedLiterals(curVar);
}

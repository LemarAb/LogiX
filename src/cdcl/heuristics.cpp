#include "../../include/cnf2.hpp"

void chooseINC() {
    while (vars[curVar].getValue() != FREE) curVar++;
    vars[curVar].setValue(TRUE);
    vars[curVar].forced = false;
    assig.push(curVar);
    // std::cout << "CHOOSE_LIT:" << curVar << "\n";
}

void chooseDLIS(){}

void chooseDLCS(){}

void chooseMOM(){}

void chooseJW(){}
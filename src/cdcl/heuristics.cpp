#include "../../include/cnf2.hpp"

void chooseINC() {
  while (vars[curVar].getValue() != FREE)
    curVar++;
  vars[curVar].setValue(TRUE);
  vars[curVar].forced = false;
  assig.push(curVar);

  //   int index = 0;
  // int max = 0;

  // for (int i = 1; i <= numOfVars; i++) {
  //   if (vars[i].getValue() != FREE)
  //     continue;

  //   if ((vars[i].neg_occ + vars[i].pos_occ) > max) {
  //     index = i;
  //     max = vars[i].neg_occ + vars[i].pos_occ;
  //   }
  // }
  // curVar = index;
  // vars[curVar].setValue(TRUE);
  // vars[curVar].forced = false;
  // assig.push(curVar);
}

void chooseDLIS() {}

void chooseDLCS() {}

void chooseMOM() {}

void chooseJW() {}
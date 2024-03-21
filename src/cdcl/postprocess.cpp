// Postprocessing for Niver preprocessing technique

#include "../../include/cdcl.hpp"
#include <algorithm>

int countSatClauses() {
  int noOfSatClauses = 0;
  for (int i = 1; i < cnf.size(); i++) {
    for (int j = 0; j < cnf[i].size(); j++) {
      if (eval(cnf[i][j])) {
        noOfSatClauses++;
        break;
      }
    }
  }
  return noOfSatClauses;
}

int niverPostprocess() {

  for (auto it = niverRemovalOrder.rbegin(); it != niverRemovalOrder.rend();
       ++it) {

    for (const auto &clause : niverRemovedClauses[*it]) {
      cnf.push_back(clause);
    }

    bool cantBeTrue = false;

    vars[*it].setValue(TRUE);

    int noOfSatClauses = countSatClauses();

    if (noOfSatClauses != cnf.size() - 1) {

      cantBeTrue = true;
      vars[*it].setValue(FALSE);
    }

    bool cantBeFalse = false;
    noOfSatClauses = countSatClauses();

    if (noOfSatClauses != cnf.size() - 1) {
      cantBeFalse = true;
    }
    if (cantBeTrue && cantBeFalse) {
      return 1;
    }
  }
  return 0;
}

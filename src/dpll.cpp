#include <string>
#include <vector>

#include "../include/cnf.hpp"
#include <iostream>
#include <queue>

bool dpll(int curVar) {
  // While loop
  std::queue<int> unitQueue;
  int varSize = variables.size();

  while (variables[curVar].val == Values::FREE || curVar == 3) {

    variables[curVar].val = Values::TRUE;
    std::cout << "Current Var :" << curVar << " and current value " << variables[curVar].val << '\n';

    // for positive occurances

    for (int i = 0, size = variables[curVar].pos_occ.size(); i < size; i++) {
      if (cnf[variables[curVar].pos_occ[i]].satLiteral == 0) {
        cnf[variables[curVar].pos_occ[i]].satLiteral = curVar;
        std::cout << "Clause " << variables[curVar].pos_occ[i]
                  << " satisfied by " << curVar << '\n';
      }
    }

    // for negative occurances

    for (int i = 0, size = variables[curVar].neg_occ.size(); i < size; i++) {
      std::cout << "in for loop 1" << '\n';
      // decrement active literals
      cnf[variables[curVar].neg_occ[i]].active--;
      std::cout << "active lit. no decremented to "
                << cnf[variables[curVar].neg_occ[i]].active << '\n';
      // if active lit. == 1
      if (cnf[variables[curVar].neg_occ[i]].active == 1) {
        std::cout << "active lit. == 1 " << '\n';
        // find unique free lit

        int clauseWidth = cnf[variables[curVar].neg_occ[i]].literals.size();
        for (int j = 0; j < clauseWidth; j++) {
          std::cout << "in for loop 2" << '\n';
          int varToCheck = cnf[variables[curVar].neg_occ[i]].literals[j];
          std::cout << "vartocheck= " << varToCheck << '\n';
          if (variables[std::abs(varToCheck)].val == Values::FREE) {
            // enqueue a
            std::cout << "in the for loop 3" << '\n';
            std::cout << "var to be added= " << varToCheck << '\n';
            unitQueue.push(varToCheck);
            while (!unitQueue.empty()) {
              int current = unitQueue.front();
              std::cout << "current queue elm= " << current << "\n";
              unitQueue.pop();
              variables[current].forced = true;
              (current > 0) ? variables[std::abs(current)].val = Values::TRUE
                            : variables[std::abs(current)].val = Values::FALSE;
              std::cout << "Value of var after setting= "
                        << variables[std::abs(current)].val << "\n";
              dpll(std::abs(current));
            }
          }
        }
      }

      // if active lit. == 0
      if (cnf[variables[curVar].neg_occ[i]].active == 1) {
        // report conflict
      }
    }

    std::cout << "queue size= " << unitQueue.size() << '\n';
    curVar++;
  }
  return true;
}

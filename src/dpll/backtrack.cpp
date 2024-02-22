#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "../../include/cnf.hpp"

void backtrack() {
  backtrackFlag = false;

  // unassign until the last branching variable.
  while (!assig.empty() && vars[assig.top()].forced) {
    btc++;
    int toUnassign = assig.top();
    revert(toUnassign);
    vars[toUnassign].val = FREE;
    vars[toUnassign].forced = false;

    assig.pop();
  }

  // clear queue of unit and pure literals
  while (!toPropagate.empty()) {
    vars[std::abs(toPropagate.front())].enqueued = false;
    toPropagate.pop();
  }

  // UNSAT
  if (assig.empty())
    pthread_exit((void *)1);

  // handle most recent branching variable
  int b = assig.top();
  revert(b);
  vars[b].forced = true;

  // assign negated val
  vars[b].val = Assig(int(2 - std::pow(2.0, vars[b].val)));
  curVar = b;
  update(b);
  propagate();
}
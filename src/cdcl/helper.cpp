#include "../../include/cdcl.hpp"

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal) { return std::abs(literal); }

void assertLit(int literal, bool forced) {
  auto &lit = vars[std::abs(literal)];
  (literal > 0) ? lit.setValue(TRUE) : lit.setValue(FALSE);
  if (forced) {
    trail.push_back(literal);
    lit.enqueued = false;
    // assig.push(std::abs(literal));
    updateWatched(std::abs(literal));
    lit.level = curDecisionLevel;
  } else {
    curDecisionLevel++;
    trail.push_back(literal);
    // assig.push(literal);
    lit.level = curDecisionLevel;
    updateWatched(literal);
  }
}

void unassignLit(int literal){
  int toUnassign = index(literal);
  vars[toUnassign].level = -1;
  vars[toUnassign].reason = -1;
  if(vars[toUnassign].getValue() != FREE)
    {
    vars[toUnassign].setValue(FREE);
    trail.pop_back();
    }
  else
    {vars[literal].enqueued = false;
      unitQueue.pop();}

}
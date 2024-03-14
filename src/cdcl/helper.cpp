#include "../../include/cdcl.hpp"

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal) { return std::abs(literal); }

void assertLit(int literal, bool forced) {
  auto &lit = vars[std::abs(literal)];
  (literal > 0) ? lit.setValue(TRUE) : lit.setValue(FALSE);
  if (forced) {
    lit.forced = true;
    trail.push_back(literal);
    lit.enqueued = false;
    // assig.push(std::abs(literal));
    updateWatched(std::abs(literal));
    lit.level = curDecisionLevel;
  } else {
    curDecisionLevel++;
    lit.forced = false;
    trail.push_back(literal);
    // assig.push(literal);
    lit.level = curDecisionLevel;
    updateWatched(literal);
  }
}
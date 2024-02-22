#include "../../include/cnf2.hpp"

void updateWatchedLiterals(int assertedVar) {
  auto clausesToUpdatePointers = &vars[assertedVar].neg_watched;

  if (vars[assertedVar].getValue() == FALSE) {
    clausesToUpdatePointers = &vars[assertedVar].pos_watched;
  }

  auto copy = *clausesToUpdatePointers;

  for (auto clauseIndex = copy.begin(); clauseIndex != copy.end();
       ++clauseIndex) {

    std::vector<int> &clause = cnf[*clauseIndex];
    bool found = false;

    if (clause.size() == 1)
      goto UP_Backtrack;

    if (eval(clause[0]) || eval(clause[1]))
      continue;

    // swap false literal to index 1
    if (std::abs(clause[1]) != assertedVar) {
      int tmp = clause[1];
      clause[1] = clause[0];
      clause[0] = tmp;
    }

    for (int i = 2; i < clause.size(); i++) {
      if (eval(clause[i]) || vars[std::abs(clause[i])].getValue() == FREE) {
        int tmp = clause[i];
        clause[i] = clause[1];
        clause[1] = tmp;

        clausesToUpdatePointers->erase(*clauseIndex);
        tmp > 0 ? vars[std::abs(tmp)].pos_watched.insert(*clauseIndex)
                : vars[std::abs(tmp)].neg_watched.insert(*clauseIndex);
        found = true;
        break;
      }
    }

  UP_Backtrack:
    if (found)
      continue;

    int otherPointer = clause[0];
    if (vars[std::abs(otherPointer)].getValue() == FREE) {
      if (!vars[std::abs(otherPointer)].enqueued) {
        unitQueue.push(otherPointer);
        vars[std::abs(otherPointer)].enqueued = true;
      }
    } else {
      backtrack();
      return;
    }
  }

  if (numOfUnassigned == 0 && unitQueue.empty())
    pthread_exit(0);
}

bool eval(int literal) {
  return !(vars[std::abs(literal)].getValue() ^ (literal > 0));
}
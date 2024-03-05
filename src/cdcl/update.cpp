#include "../../include/cdcl.hpp"

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
      goto conflict_detection;

    if (eval(clause[0]) || eval(clause[1]))
      continue;

    // swap false literal to index 1
    if (index(clause[1]) != assertedVar) {
      int tmp = clause[1];
      clause[1] = clause[0];
      clause[0] = tmp;
    }

    for (int i = 2; i < clause.size(); i++) {
      if (eval(clause[i]) || vars[index(clause[i])].getValue() == FREE) {
        int tmp = clause[i];
        clause[i] = clause[1];
        clause[1] = tmp;

        clausesToUpdatePointers->erase(*clauseIndex);
        tmp > 0 ? vars[index(tmp)].pos_watched.insert(*clauseIndex)
                : vars[index(tmp)].neg_watched.insert(*clauseIndex);
        found = true;
        break;
      }
    }

  conflict_detection:
    if (found)
      continue;

    int otherPointer = clause[0];
    if (vars[index(otherPointer)].getValue() == FREE) {
      if (!vars[index(otherPointer)].enqueued) {
        unitQueue.push(otherPointer);
        vars[index(otherPointer)].enqueued = true;
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
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal){
  return std::abs(literal);
}
#include "../../include/cdcl.hpp"
#include <cmath>

std::vector<int> trail;
bool conflict;
std::vector<int> conflict_clause;

void* cdcl(void* arg) {  // TODO: We should implement the more optimised appproach of checking the satisfaction of every clause
    while (true) {
        unitPropagate();
        // if (conflict) {
        //     if (decLevel == 0)
        //         unsat;
        //     else {
        //         conflict_analysis + backtrack
        //     }
        // } else {
        //     if (trail.size() == numofvars)
        //         unsat
        //     else {
        //         if (restarts) restart();
                pickDecisionLit();
        //     }
        // }
        
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

void pickDecisionLit() {
  while (vars[curVar].getValue() != FREE)
    curVar++;
  vars[curVar].setValue(TRUE);
  vars[curVar].forced = false;
  assig.push(curVar);
    updateWatchedLiterals(curVar);
}

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
    if (index(clause[1]) != assertedVar) 
      std::swap(clause[0], clause[1]);

    for (int i = 2; i < clause.size(); i++) {
      if (eval(clause[i]) || vars[index(clause[i])].getValue() == FREE) {
        int swapee = clause[i];
        std::swap(clause[1], clause[i]);
        clausesToUpdatePointers->erase(*clauseIndex);
        swapee > 0 ? vars[index(swapee)].pos_watched.insert(*clauseIndex)
                : vars[index(swapee)].neg_watched.insert(*clauseIndex);
        found = true;
        break;
      }
    }

  conflict_detection:
    if (found)
      continue;

    // If the first literal of the clause is free, push unitProp else conflict found
    auto & firstLit = vars[index(clause[0])];
    if (firstLit.getValue() == FREE) {
      if (!firstLit.enqueued) {
        unitQueue.push(clause[0]);
        firstLit.enqueued = true;
        firstLit.reason = *clauseIndex;      
      }
    } else {
      // build conflict clause from the two immediate conflicting clauses
      conflict = true;
      conflict_clause.clear();

      for(int lit : clause) {
        if(index(lit) != assertedVar)
          conflict_clause.push_back(lit);
      }

      for(int lit : cnf[vars[assertedVar].reason]) {
        if(index(lit) != assertedVar)
          conflict_clause.push_back(lit);
      }
      backtrack();
      return;
    }
  }

  if (numOfUnassigned == 0 && unitQueue.empty())
    pthread_exit(0);
}

void backtrack() {
    while (!assig.empty() && vars[assig.top()].forced) {  // until the last branching variable.
        int toUnassign = assig.top();
        vars[toUnassign].setValue(FREE);
        vars[toUnassign].forced = false;
        assig.pop();
        //  std::cout << "Removed literal " << toUnassign << " from assig stack \n";
    }

    // clear unit queue
    while (!unitQueue.empty()) {
        // std::cout << "Element to be popped from queue: " << unitQueue.front() << "\n";
        vars[std::abs(unitQueue.front())].enqueued = false;
        unitQueue.pop();
    }

    if (assig.empty()) {
        pthread_exit((void *)1);
    }  // UNSAT

    // Most recent branching variable
    int b = assig.top();
    // Assign negated val
    vars[b].forced = true;
    vars[b].reason = -1;
    vars[b].setValue(Assig(int(2 - std::pow(2.0, vars[b].getValue()))));
    // std::cout << "New branch var" << b << ", OLD: " << oldval << ", NEW: " << vars[b].getValue();
    curVar = b;
    updateWatchedLiterals(b);
    unitPropagate();
}

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal){
  return std::abs(literal);
}

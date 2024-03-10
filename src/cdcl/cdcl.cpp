#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include <cmath>

std::vector<int> trail;
bool conflict;
std::vector<int> conflict_clause;
std::set<int> UIP_set;
int curDecisionLevel = 0;

void* cdcl(void* arg) {
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
        trail.push_back(std::abs(unitLiteral));
        updateWatchedLiterals(std::abs(unitLiteral));
    }
}

void pickDecisionLit() {
  while (vars[curVar].getValue() != FREE)
    curVar++;
  vars[curVar].setValue(TRUE);
  vars[curVar].forced = false;
  assig.push(curVar);
  trail.push_back(curVar);
  curDecisionLevel++;
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
      conflict = true;
      conflict_clause.clear();

      /*_________________________________________________________________________________________________
      |
      | Upon conflict, we first want to identify all of the vertices of the current decision level
      | adjacent to the conflict vertex.
      | 
      | This will be the set of literals except for 'assertedVar' that occur in the falsified clause  
      | and the propagated clauses carrying the 'assertedVar' and that share the current decision level.
      | 
      | The resulting 'UIP_set' will be used in the conflict analysis step to determine the 1-UIP.
      |_________________________________________________________________________________________________*/ 

      for(int lit : clause) {
        if(index(lit) != assertedVar && vars[index(lit)].level == curDecisionLevel)
          UIP_set.insert(lit);
      }

      for(int lit : cnf[vars[assertedVar].reason]) {
        if(index(lit) != assertedVar && vars[index(lit)].level == curDecisionLevel)
          UIP_set.insert(lit);
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

void build_conflict_clause(){
  
  int uip = findUIP();

}

int findUIP(){
  int toPop = trail.size();
  while(UIP_set.size()>1){
    if(UIP_set.find(trail[toPop]) != UIP_set.end())
      trail[toPop] = 0;
  }
}

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal){
  return std::abs(literal);
}



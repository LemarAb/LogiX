#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include <cmath>

std::vector<std::pair<int, int>> trail;
bool conflict;
std::vector<int> conflict_clause;
int count_lits_highest_dec_level = 1;
int curDecisionLevel = 0;
int *seen = new int[numOfVars];

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal) { return std::abs(literal); }

void assertLit(int literal, bool forced) {
  auto &lit = vars[std::abs(literal)];
  (literal > 0) ? lit.setValue(TRUE) : lit.setValue(FALSE);

  if (forced) {
    lit.forced = true;
    trail.push_back(std::make_pair(literal, curDecisionLevel));
    lit.enqueued = false;
    assig.push(std::abs(literal));
    updateWatchedLiterals(std::abs(literal));
  } else {
    lit.forced = false;
    trail.push_back(std::make_pair(literal, ++curDecisionLevel));
    assig.push(literal);
    updateWatchedLiterals(literal);
  }
}

void *cdcl(void *arg) {
  while (true) {
    unitPropagate();
    if(conflict){
      backtrack();
    }
    else {
      pickDecisionLit();
    }
  }
}

void unitPropagate() {
  int unitLiteral;
  while (!unitQueue.empty() && !conflict) {
    unitLiteral = unitQueue.front();
    unitQueue.pop();
    assertLit(unitLiteral, true);
  }
}

void pickDecisionLit() {
  while (vars[curVar].getValue() != FREE)
    curVar++;
  assertLit(curVar, false);
}

void updateWatchedLiterals(int assertedLit) {

  auto clausesToUpdatePointers = &vars[assertedLit].neg_watched;

  if (vars[assertedLit].getValue() == FALSE) {
    clausesToUpdatePointers = &vars[assertedLit].pos_watched;
  }

  auto copy = *clausesToUpdatePointers;

  for (auto clauseIndex = copy.begin(); clauseIndex != copy.end();
       ++clauseIndex) {

    std::vector<int> &clause = cnf[*clauseIndex];
    bool found = false;

    if (eval(clause[0]) || eval(clause[1]))
      continue;

    // swap false literal to index 1
    if (index(clause[1]) != assertedLit)
      std::swap(clause[0], clause[1]);
    auto &unitLit = vars[index(clause[0])];
    for (int i = 2; i < clause.size(); i++) {
      if (eval(clause[i]) || vars[index(clause[i])].getValue() == FREE) {
        int swapee = clause[i];
        std::swap(clause[1], clause[i]);
        clausesToUpdatePointers->erase(*clauseIndex);
        swapee > 0 ? vars[index(swapee)].pos_watched.insert(*clauseIndex)
                   : vars[index(swapee)].neg_watched.insert(*clauseIndex);
        goto nextIter;
      }
    }

    // If the first literal of the clause is free, push unitProp else conflict
    // found
    
    if (unitLit.getValue() == FREE) {
      if (!unitLit.enqueued) {
        unitQueue.push(clause[0]);
        unitLit.enqueued = true;
      }
    } else {
      // CONFLICT!
      //  int backtrack_lvl = analyze(*clauseIndex);
      //  backjump();
      conflict = true;
      // backtrack();
      return;
    }
  nextIter:;
  }

  if (numOfUnassigned == 0 && unitQueue.empty())
    pthread_exit(0);
}

// int analyze(int conf_clause_id){

//   int numOfLits = 1; // number of literals of current decision level in our
//   conflict clause int trailIndex   = trail.size() - 1;
//   conflict_clause.push_back(-1); // push dummy, replaced after loop
//   std::pair<int, int> seenLit;
//   while(numOfLits > 1){
//     std::vector<int>& conf = cnf[conf_clause_id];
//     for(int i = 1; i < conf.size(); i++){
//       int lit = conf[i];
//       if (!seen[index(lit)] && vars[index(lit)].level > 0){
//                 // varBumpActivity(var(q));
//                 seen[index(lit)] = 1;
//                 if (vars[index(lit)].level >= curDecisionLevel)
//                     numOfLits++;
//                 else
//                     conflict_clause.push_back(lit);
//         }
//     }

//     // traverse trail until we encounter a seen lit
//     while (!seen[trail[trailIndex].first])
//       trailIndex--;

//     seenLit = trail[trailIndex];
//     conf_clause_id = seenLit.second;
//     seen[seenLit.first] = 0;
//     numOfLits--;
//   }

//   conflict_clause[0] = seenLit.first;
//   int btlvl;
//   if (conflict_clause.size() == 1)
//         btlvl = 0;
//     else{
//         // index of the second highest dec level literal
//         int max = 1;
//         // Find the first literal assigned at the second highest level:
//         for (int i = 2; i < conflict_clause.size(); i++)
//             if (vars[index(conflict_clause[i])].level >
//             vars[index(conflict_clause[max])].level)
//                 max = i;
//         btlvl = vars[index(conflict_clause[max])].level;
//     }
//   return btlvl;
// }

void backtrack() {
  while (!assig.empty() &&
         vars[assig.top()].forced) { // until the last branching variable.
    int toUnassign = assig.top();
    vars[toUnassign].setValue(FREE);
    vars[toUnassign].forced = false;
    assig.pop();
    //  std::cout << "Removed literal " << toUnassign << " from assig stack \n";
  }

  conflict = false;

  // clear unit queue
  while (!unitQueue.empty()) {
    // std::cout << "Element to be popped from queue: " << unitQueue.front() <<
    // "\n";
    vars[std::abs(unitQueue.front())].enqueued = false;
    unitQueue.pop();
  }

  if (assig.empty()) {
    pthread_exit((void *)1);
  } // UNSAT

  // Most recent branching variable
  int b = assig.top();
  // Assign negated val
  vars[b].forced = true;
  // vars[b].reason.clear();
  vars[b].setValue(Assig(int(2 - std::pow(2.0, vars[b].getValue()))));
  // std::cout << "New branch var" << b << ", OLD: " << oldval << ", NEW: " <<
  // vars[b].getValue();
  curVar = b;
  updateWatchedLiterals(b);
  unitPropagate();
}

#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include <cmath>

std::vector<int> trail;
bool conflict;
std::vector<int> conflict_clause;
int count_lits_highest_dec_level = 1;
int curDecisionLevel = 0;
bool *seen = new bool[numOfVars];
int conflict_clause_id;

void *cdcl(void *arg) {
  while (true) {
    unitPropagate();
    if(conflict){
      // int backtrack_lvl = analyze();
      //       for(int i = 0; i < conflict_clause.size(); i++){
      //   printf("%i ", conflict_clause[i]);
      // }
      //   printf("\n");
      backtrack(curDecisionLevel);

      // pthread_exit(0);
      //  backjump();
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

void updateWatched(int assertedLit) {

  auto watchedClauses = &vars[assertedLit].neg_watched;

  if (vars[assertedLit].getValue() == FALSE) {
    watchedClauses = &vars[assertedLit].pos_watched;
  }

  auto copy = *watchedClauses;

  for (auto clauseIndex = copy.begin(); clauseIndex != copy.end();
       ++clauseIndex) {

    std::vector<int> &clause = cnf[*clauseIndex];

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
        watchedClauses->erase(*clauseIndex);
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
        unitLit.reason = *clauseIndex;
      }
    } else {
      // CONFLICT!
      // for(int i = 0; i < clause.size(); i++)
      //   printf("CONF!: %i, %i, %i\n", *clauseIndex, clause[i], vars[index(clause[i])].reason);
   
      conflict_clause_id = *clauseIndex;
      conflict = true;
      return;
    }
  nextIter:;
  }

  if (numOfUnassigned == 0 && unitQueue.empty())
    pthread_exit(0);
}

int analyze() {

  conflict_clause.clear();
  int numOfLits = 0; // number of literals of current decision level in our
  int trailIndex = trail.size() - 1;
  conflict_clause.push_back(-1); // push dummy, replaced after loop
  int stampedLit = -1;
    printf("HIER!: %i\n", trailIndex);
  do{
    std::vector<int>& conf = cnf[conflict_clause_id];
    for(int i = (stampedLit == -1) ? 0 : 1; i < conf.size(); i++){
      int lit = conf[i];
      if (!seen[index(lit)] && vars[index(lit)].level > 0){
                // varBumpActivity(var(q));
                seen[index(lit)] = 1;
                printf("HIERMITTEEVOR!: %i\n", vars[index(stampedLit)].reason);
                if (vars[index(lit)].level >= curDecisionLevel)
                    {numOfLits++;}
                else
                    {conflict_clause.push_back(lit);}
      }
      printf("HIER2!: %i\n", lit);
    }
    // traverse trail until we encounter a seen lit
    while (!seen[trail[trailIndex]])
      trailIndex--;
    stampedLit = trail[trailIndex];
    conflict_clause_id = vars[index(stampedLit)].reason;
    seen[index(stampedLit)] = 0;
    numOfLits--;
  } while(numOfLits > 0);
  conflict_clause[0] = -stampedLit;
  return 1;

  
  // int btlvl;
  // if (conflict_clause.size() == 1)
  //       btlvl = 0;
  //   else{
  //       // index of the second highest dec level literal
  //       int max = 1;
  //       // Find the first literal assigned at the second highest level:
  //       for (int i = 2; i < conflict_clause.size(); i++)
  //           if (vars[index(conflict_clause[i])].level >
  //           vars[index(conflict_clause[max])].level)
  //               max = i;
  //       btlvl = vars[index(conflict_clause[max])].level;
  //   }
  // return btlvl;
}

void backtrack(int btlvl) {
  int penultimate = trail.size()-2;

  // std::cout << "btleevel" << btlvl << "\n";

  while (trail.size() > 1 && vars[index(trail[penultimate--])].level >= btlvl)
    unassignLit(trail.back());
    // std::cout << "Removed literal " << toUnassign << "on lvl"<< vars[toUnassign].level <<" from assig stack  \n";

  conflict = false;

  // clear unit queue
  while (!unitQueue.empty())
    {
    // std::cout << "Element to be popped from queue: " << unitQueue.front() <<
    // "\n";
    int toDiscard = index(unitQueue.front());
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = -1;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }

  // UNSAT
  if (trail.size()==1 && vars[index(trail.front())].level == 0)
    pthread_exit((void *)1);

  int b = index(trail.back()); // Most recent branching variable

  vars[b].reason = 0;
  vars[b].setValue(Assig(vars[b].getValue() == 1 ? 0 : 1)); //  Flip assignment
  curDecisionLevel = --vars[b].level;
  curVar = b;

  updateWatched(b);
}

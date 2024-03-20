#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include <cmath>

std::vector<int> trail;
bool conflict;
std::vector<int> learned;
int count_lits_highest_dec_level = 1;
int curDecisionLevel = 0;
std::vector<int> seen;
int conflict_clause_id;
std::vector<int> unitTrail;

void *cdcl(void *arg) {
  while (true) {
    unitPropagate();
    // printf("unit");
    // printf("%i", trail.size());

    // printf("past unit");

    if (conflict) {
      // printf("enter conf");

      if (curDecisionLevel == 0)
        pthread_exit((void *)1);

      int backtrack_lvl = analyze();
      // if(backtrack_lvl == 0 ){
      //   backtrack2();

      //                            }
      // else{
      // addClause(learned);
      backtrack(curDecisionLevel);

      // // pthread_exit(0);
      // //  backjump();
      // }
    } else {
      pickDecisionLit();
    }
  }
}

void unitPropagate() {
  int unitLiteral;
  while (!unitQueue.empty() && !conflict) {
    unitLiteral = unitQueue.front().literal;
    assertLit(unitLiteral, true);

    // printf("unit queu: %i", unitLiteral);
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
        unitQueue.push(Unit(clause[0], *clauseIndex));
        unitLit.enqueued = true;
      }
    } else {
      // CONFLICT!
      // for(int i = 0; i < trail.size(); i++)
      //   printf("ASSIG!: %i,", (trail[i]));

      // printf("\n");
      // for(int lit : cnf[*clauseIndex]) printf("%i: %i ,", lit,
      // vars[index(lit)].getValue());
      //         printf("\n");

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

  learned.clear();
  // int pathC = 0; // number of literals of current decision level in our
  // int trailIndex = trail.size() - 1;
  learned.push_back(-1); // push dummy, replaced after loop
  // int stampedLit = -1;

  std::vector<int> &confl = cnf[conflict_clause_id];
  //   printf("Conflict");
  //  for (int i : confl)
  //     printf("%i ", i);
  //   printf("\n");
  for(int i =  0 ; i < confl.size(); i++){
    int lit = confl[i];

    // If decision lit, we learn immediately
    if(vars[index(lit)].reason <= 0) {
      if(!seen[index(lit)]){     
      learned.push_back(lit);
      seen[index(lit)] = 1;
      }
      continue;
    }

    std::vector<int> &reason = cnf[vars[index(lit)].reason];
    // for (int i : reason)
    //   printf("%i ", i);
    // printf("\n");
    for(int j = 1; j < reason.size(); j++) {
      // Do not include 0 level literals in the learned
      int toCache = reason[j];
      if(!seen[index(toCache)] && vars[index(toCache)].level > 0) {
        learned.push_back(toCache);
        seen[index(toCache)] = 1;
      }
    }
  }
    // printf("Ballert: \n");
    // for (int i : learned)
    //   printf("%i ", i);
    // printf("0\n");

  int btlvl;
  if (learned.size() == 1)
    btlvl = 0;
  else {
    // index of the second highest dec level literal
    int max = 1;
    // Find the first literal assigned at the second highest level:
    for (int i = 2; i < learned.size(); i++)
      if (vars[index(learned[i])].level >
          vars[index(learned[max])].level)
        max = i;
    btlvl = vars[index(learned[max])].level;
  }

  for (int elem = 0; elem < seen.size(); elem++) {
    seen[index(elem)] = false;
  }
  return 1;
}

void backtrack(int btlvl) {
  int penultimate = trail.size() - 2;

  // std::cout << "btleevel" << btlvl << "\n";

  while (trail.size() > 1 && vars[index(trail[penultimate--])].level >= btlvl)
    unassignLit(trail.back());
  // std::cout << "Removed literal " << toUnassign << "on lvl"<<
  // vars[toUnassign].level <<" from assig stack  \n";

  conflict = false;

  // clear unit queue
  while (!unitQueue.empty()) {
    // std::cout << "Element to be popped from queue: " << unitQueue.front() <<
    // "\n";
    int toDiscard = index(unitQueue.front().literal);
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = -1;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }

  // UNSAT
  // if (trail.size()==1 && vars[index(trail.front())].level == 0)
  //   pthread_exit((void *)1);

  // if(btlvl==0) {
  // unassignLit(trail.back());
  // curDecisionLevel = 0;
  // for(int unit : unitTrail){
  //   unitQueue.push(unit);
  //   vars[index(unit)].enqueued = true;
  //   // unitTrail.push_back(unit);
  // }
  // // unitQueue.push(learned[0]);
  // // vars[index(learned[0])].enqueued = true;
  // // unitTrail.push_back(learned[0]);
  // curVar = 1;
  // }
  // else{

  int b = index(trail.back()); // Most recent branching variable

  vars[b].reason = -1;
  vars[b].setValue(Assig(vars[b].getValue() == 1 ? 0 : 1)); //  Flip assignment
  curDecisionLevel = --vars[b].level;
  curVar = b;

  updateWatched(b);
}

void backtrack2() {
  // printf("HIER %i\n", curDecisionLevel);
  // printf("Trail: %i", trail.size());

  while (!trail.empty() && vars[index(trail.back())].level != 0)
    unassignLit(trail.back());
  // printf("Trail: %i", trail.size());
  // for(int lit : trail) printf("%i: %i,", lit, vars[index(lit)].level);

  while (!unitQueue.empty()) {
    // std::cout << "Element to be popped from queue: " << unitQueue.front() <<
    // "\n";
    int toDiscard = index(unitQueue.front().literal);
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = -1;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }

  conflict = false;

  unitQueue.push(Unit(learned[0], -1));
  vars[index(learned[0])].enqueued = true;
  unitTrail.push_back(learned[0]);

  curDecisionLevel = 0;

  curVar = 1;

  // printf("HIER %i, %i\n", learned[0], curVar);
}

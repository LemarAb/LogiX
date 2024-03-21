#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include <cmath>
#include "dataStructs/vsids.hpp"

std::vector<int> trail;
bool conflict;
std::vector<int> learned;
int count_lits_highest_dec_level = 1;
int curDecisionLevel = 0;
std::vector<int> seen;
int conflict_clause_id;
std::vector<int> unitTrail;
int conflict_count = 0;
int delete_cue = 0;
int decision_count = 0;

void *cdcl(void *arg) {
  createHeap();
  while (true) {
    unitPropagate();
    // printf("post");
    // printf("unit");
    // printf("%i", decision_vars.size());

    // printf("past unit");

    if (conflict) {
      // printf("enter conf");
      conflict_count++;
      if (curDecisionLevel <= 0)
        pthread_exit((void *)1);

      int backtrack_lvl = analyze();
      if(backtrack_lvl == 0 ){
        printf("WOW%i", learned[0]);

                                 }
      else{
        addClause(learned);
        
      }
      // if (conflict_count == luby(luby_index) * luby_unit) {
      //   printf("%i",luby(luby_index)*luby_unit);
      //   luby_index++;
      //   // restart();
      //   restart();
      //   continue;
      // }

      // if (conflict_count >= pow(delete_cue, 2)){
      //   printf("del%i und 2 hoch %i\n", conflict_count, delete_cue);
      //   delete_half();
      //   delete_cue++;
      // }

      backtrack(backtrack_lvl);

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

  decision_count++;

  if((decision_count % 255) == 0) allVarsHalfActivity();

  while(vars[ heap.peek() ].getValue() != FREE)
    heap.removeMax();

  curVar = heap.removeMax();

  // while (vars[curVar].getValue() != FREE)
  //   curVar++;
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
      for(int lit : cnf[*clauseIndex]) varIncActivity(index(lit));
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
  // learned.push_back(-1); // push dummy, replaced after loop
  // int stampedLit = -1;

  std::vector<int> &confl = cnf[conflict_clause_id];
  //  for (int i : confl){
  //     printf("%i: ", i);
  //     if(vars[index(i)].reason > 0){
  //       for(int j = 0; j < cnf[vars[index(i)].reason].size(); j++){
  //         printf("%i, %i; ", cnf[vars[index(i)].reason][j], vars[index (cnf[vars[index(i)].reason][j])].reason);
  //       }}
  //   printf(".....REASON %i\n", vars[index(i)].reason );}


        // printf("%i ", i);
  for(int i =  0 ; i < confl.size(); i++){
    int lit = confl[i];

    // If decision lit, we learn immediately, if forced literal skip
    if(vars[index(lit)].reason <= 0) {
      if(!seen[index(lit) && vars[index(lit)].reason == 0]){     
      learned.push_back(lit);
      }
      seen[index(lit)] = 1;
      continue;
      
    }
      
    std::vector<int> &reason = cnf[vars[index(lit)].reason];
    // for (int i : reason)
    //   printf("%i ", i);
    // printf("\n");
    for(int j = 1; j < reason.size(); j++) {
      // Do not include 0 level literals in the learned
      
// printf("Conflict %i\n", reason.size());
      int toCache = reason[j];
      if(!seen[index(toCache)] && vars[index(toCache)].reason >= 0) {
        
        learned.push_back(toCache);
      }
      
      seen[index(toCache)] = 1;
    }
  }
  //   if(learned.size()==1)
  // {  printf("Ballert: \n");
  //     for (int i : learned)
  //       printf("ONE!%i ", i);
  //   printf("0\n");}

  int btlvl;
  if (learned.size() == 1)
    btlvl = 0;
  else {
    // index of the second highest dec level literal
    int max = 0;
    // Find the first literal assigned at the second highest level:
    for (int i = 1; i < learned.size(); i++)
      if (vars[index(learned[i])].level >
          vars[index(learned[max])].level)
        max = i;
    btlvl = vars[index(learned[max])].level;
  // if(btlvl != curDecisionLevel)
  // printf("BT: %i, CDL: %i\n", btlvl, curDecisionLevel);
  }
  // printf("BT: %i, CDL: %i\n", btlvl, curDecisionLevel);
        // for (int i : learned)
        // printf("%i ", i);
        //         printf("0\n");

  for (int elem = 0; elem < seen.size(); elem++) {
    seen[index(elem)] = false;
  }
  return btlvl;
}

void backtrack(int btlvl) {
  // btlvl = 0;
  //  for (int i : trail)
  //     printf("%i: %i, ", i, vars[index(i)].level);
  //   printf("\n\n%i, %i, %i\n\n", decision_vars[btlvl], curDecisionLevel, decision_vars.size());
    
  bool addedClause = btlvl > 0;
  if(btlvl == 0) {
        

      while (!trail.empty() && vars[index(trail.back())].reason >= 0)
    {unassignLit(trail.back());

    }
  }
else
 { while (!trail.empty() && (index(trail.back()) != decision_vars[btlvl]))
    {unassignLit(trail.back());
    
    }}
  conflict = false;


  // clear unit queue
  while (!unitQueue.empty()) {
    int toDiscard = index(unitQueue.front().literal);
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = 0;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }
  if (addedClause) {
    int b = trail.back();
    unassignLit(b);
    //TODO
    unitQueue.push(Unit(-b, cnf.size()-1));
    vars[index(b)].enqueued = true;
  }

  else {
    unitQueue.push(Unit(learned[0], -1));
    vars[index(learned[0])].enqueued = true;
  }
  //  for (int i : trail)
  //     printf("%i: %i, ", i, vars[index(i)].level);
  //   printf("%i", trail.size());
  int diff = curDecisionLevel - btlvl + 1 - !addedClause;
  while(diff > 0) {decision_vars.pop_back(); diff--;}

  curDecisionLevel = decision_vars.size()-1;
  if (curDecisionLevel == 0) curVar = 1;
  else curVar = decision_vars[curDecisionLevel];

  //    for (int i : trail)
  //     printf("%i: %i, ", i, vars[index(i)].level);
  // printf("\n CURVAR: %i, %i, %i\n", curVar, curDecisionLevel,  decision_vars.back());
          // pthread_exit((void *)1);

}

void restart() {
  // printf("RESTART\n");

  // printf("HIER %i\n", curDecisionLevel);

  // printf("trail size %i\n", trail.size());
  // printf("phase size %i\n", phase.size());
  // printf("numofvars %i\n", numOfVars);
  // for (int i = 0; i < phase.size(); i++) {
  // printf("free");
  //  phase[i] = FREE;
  //}
  // for (int i = 0; i < trail.size(); i++) {
  //   printf("%i: %i, ", i, trail[i]);
  // }
  // printf("\n");
  // for(int i = 0; i < phase.size(); i++) phase[i] = FREE;
  while (!trail.empty() && vars[index(trail.back())].reason >= 0) {
    // heap.insert(index(trail.back()));

    int toSave = index(trail.back());
    // phase[toSave] = vars[toSave].getValue();
    unassignLit(trail.back());
  }

  while (!unitQueue.empty()) {
    int toDiscard = index(unitQueue.front().literal);
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = 0;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }

  conflict = false;

  while (decision_vars.size() > 1) {
    decision_vars.pop_back();
  }

  // unitQueue.push(conflict_clause[0]);
  // vars[index(conflict_clause[0])].enqueued = true;
  // unitTrail.push_back(conflict_clause[0]);

  conflict_count = 0;

  curDecisionLevel = 0;

  curVar = 1;

  heap.rebuild();

}

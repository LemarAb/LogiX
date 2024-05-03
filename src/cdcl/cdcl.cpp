#include "../../include/cdcl.hpp"
#include "../../src/cdcl/Heap/vsids.hpp"
#include "Heap/vsids.hpp"
#include <cmath>

std::vector<int> trail;
bool conflict;
std::vector<int> learned;
int curDecisionLevel = 0;
std::vector<int> seen;
int conflict_clause_id = 0;
std::vector<int> unitTrail;
int conflict_count = 0;
int decision_count = 0;

bool cdcl() {

  createHeap();

  while (true) {
    unitPropagate();

    if (conflict) {

      conflict_count++;

      if (curDecisionLevel <= 0)
        return false;

      if (conflict_count == luby(luby_index) * luby_unit) {
        luby_index++;
        restart();
        continue;
      }

      int backtrack_lvl = analyze();

      if (backtrack_lvl > 0) {
          addClause(learned);
          unitQueue.push(Unit(learned[0], cnf.size()-1));
      }
      
      else {
          learnedUnits.push_back(learned[0]);
          unitQueue.push(Unit(learned[0], -1));
      }  

      backtrack(backtrack_lvl);


      // if (conflict_count >= pow(2, delete_cue)){
      //   delete_half();
      //   delete_cue++;
      //  }


    } else {
    
    if(trail.size() == numOfVars)
      return true;

    int decision_lit = pickDecisionLit();
    assertLit(decision_lit, false);

    }
  }
}

void unitPropagate() {
  int unitLiteral;
  while (!unitQueue.empty() && !conflict) {
    unitLiteral = unitQueue.front().literal;
    assertLit(unitLiteral, true);
  }
}

int pickDecisionLit() {

  decision_count++;

  // every 255th decision, half all var activity
  if ((decision_count % 256) == 0)
    allVarsHalfActivity();

  // get the most relevant unassigned var from the max heap
  while (vars[heap.peek()].getValue() != FREE)
    heap.removeMax();

  return heap.removeMax();
}

void updateWatched(int assertedLit) {

  auto watchedClauses = &vars[assertedLit].neg_watched;

  if (vars[assertedLit].getValue() == FALSE)
    watchedClauses = &vars[assertedLit].pos_watched;


  auto copy = *watchedClauses;

  for (auto clauseIndex = copy.begin(); clauseIndex != copy.end();
       ++clauseIndex) {

    std::vector<int> &clause = cnf[*clauseIndex];

    // watched literals are kept at indices 0 and 1
    if (eval(clause[0]) || eval(clause[1]))
      continue;

    // swap falsified literal to index 1
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

    //
    if (unitLit.getValue() == FREE) {
      if (!unitLit.enqueued) {
        int reason = *clauseIndex;
        unitQueue.push(Unit(clause[0], reason));
        unitLit.enqueued = true;
      }
    } else {
      conflict_clause_id = *clauseIndex;
      conflict = true;
      emptyUnitQueue();
      return;
    }
  nextIter:;
  }
}

int analyze() {

  learned.clear();

  int num = 0; // number of literals of current decision level in our
  int trailIndex = trail.size() - 1;
  learned.push_back(-1); // push dummy, replaced after loop
  int stampedLit;
  bool firstIter = true;
  do {
    std::vector<int> &conf = cnf[conflict_clause_id];
    for (int i = (firstIter) ? 0 : 1; i < conf.size(); i++) {
      int lit = conf[i];
      if (!seen[index(lit)] && vars[index(lit)].level > 0) {
        
        seen[index(lit)] = 1;
        if (vars[index(lit)].level >= curDecisionLevel)
          num++;
        else
          learned.push_back(lit);
      }
    }

    // traverse trail until we encounter a seen lit
    firstIter = false;
    while (!seen[index(trail[trailIndex--])]);
    stampedLit = trail[trailIndex + 1];
    seen[index(stampedLit)] = 0;
    num--;
    conflict_clause_id = vars[index(stampedLit)].reason;

  } while (num > 0);
  learned[0] = -stampedLit;

  int btlvl;
  if (learned.size() == 1)
    btlvl = 0;
  else {
    int max = 1;
    // Find the first literal assigned at the second highest level:
    for (int i = 2; i < learned.size(); i++)
      if (vars[index(learned[i])].level > vars[index(learned[max])].level)
        max = i;
    btlvl = vars[index(learned[max])].level + 1;

    // Uphold watched literal constraint (first two lits should be free)
    std::swap(learned[1], learned[max]);
  }

  for (int elem : learned)
    seen[index(elem)] = 0;
  
  return btlvl;
}

void backtrack(int btlvl) {

  conflict = false;

  if (btlvl == 0) btlvl = 1;

  while (!trail.empty() && vars[index(trail.back())].level >= btlvl)
    unassignLit(trail.back());

  curDecisionLevel = btlvl - 1;
  
}

void restart() {  
  
  conflict = false;

  for (int i = 0; i < phase.size(); i++)
    phase[i] = FREE;

  while (!trail.empty() && vars[index(trail.back())].level > 0) {
    // save current assigs in the phase vector
    int toSave = index(trail.back());
    phase[toSave] = vars[toSave].getValue();
    unassignLit(trail.back());
  }
  
  conflict_count = 0;

  curDecisionLevel = 0;
}

void addClause(std::vector<int> &clause) {

  cnf.push_back(clause);

  // VSIDS: Bump activity scores of all literals of the learned clause
  for (int lit : clause)
    varIncActivity(index(lit));

  // assign watched literals for the learned clause
  clause[0] > 0 ? vars[index(clause[0])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[0])].neg_watched.insert(cnf.size() - 1);

  clause[1] > 0 ? vars[index(clause[1])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[1])].neg_watched.insert(cnf.size() - 1);
}
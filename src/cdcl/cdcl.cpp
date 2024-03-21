#include "../../include/cdcl.hpp"
#include "../../src/cdcl/dataStructs/vsids.hpp"
#include "dataStructs/vsids.hpp"
#include <cmath>

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

    if (conflict) {

      conflict_count++;

      if (curDecisionLevel <= 0)
        pthread_exit((void *)1);

      int backtrack_lvl = analyze();

      if (backtrack_lvl > 0)
        addClause(learned);
      
      if (conflict_count == luby(luby_index) * luby_unit) {
        luby_index++;
        restart();
        continue;
      }

      // Clause deletion: Does not deliver reliable improvement

      // if (conflict_count >= pow(delete_cue, 2)){ 
      //   delete_half();
      //   delete_cue++;
      //  }

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
  }
}

void pickDecisionLit() {

  decision_count++;

  // every 255th decision, half all var activity
  if ((decision_count % 255) == 0)
    allVarsHalfActivity();

  // get the most 
  while (vars[heap.peek()].getValue() != FREE)
    heap.removeMax();

  curVar = heap.removeMax();

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
        int ci = *clauseIndex;
        unitQueue.push(Unit(clause[0], ci));
        unitLit.enqueued = true;
      }
    } else {
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

  int btlvl;
  if (learned.size() == 1)
    btlvl = 0;
  else {
    // index of the second highest dec level literal
    int max = 1;
    // Find the first literal assigned at the second highest level:
    for (int i = 2; i < learned.size(); i++)
      if (vars[index(learned[i])].level > vars[index(learned[max])].level)
        max = i;
    btlvl = vars[index(learned[max])].level;

    std::swap(learned[0], learned[max]); ??
  }

  for (int elem = 0; elem < seen.size(); elem++) {
    seen[index(elem)] = false;
  }

  // empty unit queue
  while (!unitQueue.empty()) {
  int toDiscard = index(unitQueue.front().literal);
  vars[toDiscard].enqueued = false;
  vars[toDiscard].reason = 0;
  vars[toDiscard].level = -1;
  unitQueue.pop();
}

  return btlvl;
}

void backtrack(int btlvl) {

  bool addedClause = btlvl > 0;
  if (btlvl == 0) {
    while (!trail.empty() && vars[index(trail.back())].reason >= 0) {
      heap.insert(index(trail.back()));
      unassignLit(trail.back());
    }
  } else {
    while (!trail.empty() && (index(trail.back()) != decision_vars[btlvl])) {
      heap.insert(index(trail.back()));
      unassignLit(trail.back());
    }
  }

  conflict = false;

  if (addedClause) {
    int b = trail.back();
    unassignLit(b);
  }

  else {
    // If we learn a unit clause, we did not add it to the CNF and just assert it on dec level 0
    unitQueue.push(Unit(learned[0], -1));
    vars[index(learned[0])].enqueued = true;
  }

  int diff = curDecisionLevel - btlvl + 1 - !addedClause;
  while (diff > 0) {
    decision_vars.pop_back();
    diff--;
  }

  curDecisionLevel = decision_vars.size() - 1;
}

void restart() {

  for(int i = 0; i < phase.size(); i++) phase[i] = FREE;
  while (!trail.empty() && vars[index(trail.back())].reason >= 0) {
    // save current assigs in the phase vector
    int toSave = index(trail.back());
    phase[toSave] = vars[toSave].getValue();
    unassignLit(trail.back());
  }
  // empty unit queue
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

  conflict_count = 0;

  curDecisionLevel = 0;

  heap.rebuild();
}

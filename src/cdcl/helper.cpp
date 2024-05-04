#include "../../include/cdcl.hpp"
#include "Heap/heap.hpp"
#include "Heap/vsids.hpp"
#include <algorithm>
#include <cmath>

int learned2power = 0;
std::vector<double> act;

VariableOrderActLT lt(act);

Heap<int, VariableOrderActLT> heap(lt);

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal) { return std::abs(literal); }

bool isFree(int literal) { return vars[index(literal)].getValue() == FREE; }

int luby_index = 1;

int luby_unit = 32;

int luby(int i) {
  int k;
  for (k = 0; (1 << (k + 1) <= (i + 1)); k++)
    ;
  if ((1 << k) == (i + 1))
    return (1 << (k - 1));
  else
    return luby((i + 1) - (1 << k));
}

void delete_half() {
  int learned_half = learned_begin + delete_cue / 2;
  int i; 
  
  for (i = learned_begin; i < learned_half; i++) {
    vars[index(cnf[i][0])].pos_watched.erase(i);
    vars[index(cnf[i][1])].neg_watched.erase(i);
  }

  learned_begin = i;
}

void assertLit(int literal, bool forced) {
  auto &lit = vars[std::abs(literal)];

  if (forced) {
    (literal > 0) ? lit.setValue(TRUE) : lit.setValue(FALSE);
    trail.push_back(literal);
    lit.enqueued = false;
    vars[index(literal)].reason =
        curDecisionLevel == 0 ? -1 : unitQueue.front().reason;
    unitQueue.pop();
    lit.level = curDecisionLevel;

  } else {
    bool isPhaseFalse = phase[index(literal)] == 0;

    if (isPhaseFalse){
      lit.setValue(FALSE);
      trail.push_back(-literal);
    }
    else{
      lit.setValue(TRUE);
      trail.push_back(literal);
    }

    lit.level = ++curDecisionLevel;
    lit.reason = 0;
    
  }

  updateWatched(index(literal));

  auto watchedClauses = &vars[index(literal)].neg_watched;

  if (vars[index(literal)].getValue() == FALSE)
    watchedClauses = &vars[index(literal)].pos_watched;
  

  while(!del_ref.empty()) {watchedClauses->erase(del_ref.front());  del_ref.pop();}
}

void unassignLit(int literal) {
  int toUnassign = index(literal);

  heap.insert(toUnassign);
  vars[toUnassign].level = -1;
  vars[toUnassign].reason = 0;
  if (vars[toUnassign].getValue() != FREE) {
    vars[toUnassign].setValue(FREE);
    trail.pop_back();
  } else {
    vars[literal].enqueued = false;
    unitQueue.pop();
  }
}

void emptyUnitQueue() {
  while (!unitQueue.empty()) {
    int toDiscard = index(unitQueue.front().literal);
    vars[toDiscard].enqueued = false;
    vars[toDiscard].reason = 0;
    vars[toDiscard].level = -1;
    unitQueue.pop();
  }

}

double var_inc = 1.0;

void createHeap() {
  act.resize(numOfVars + 1);
  for (int i = 1; i <= numOfVars; i++) {
    if (i < act.size() && i < vars.size())
      act[i] = vars[i].tot_occ;
  }

  heap.createHeap();
}

void varIncActivity(int var) {
  act[var] += var_inc;
  heap.update(var);
};

void varDecActivity(int var) {
  act[var] = std::max(act[var]-var_inc, 1.0);
  heap.update(var);
};

void allVarsHalfActivity() {
  for (int i = 1; i <= numOfVars; i++) {
    act[i] *= 0.5;
    heap.update(i);
  }
};

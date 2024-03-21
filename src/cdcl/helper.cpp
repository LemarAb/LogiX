#include "../../include/cdcl.hpp"
#include "dataStructs/heap.hpp"
#include "dataStructs/vsids.hpp"
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

void addClause(std::vector<int> &clause) {

  cnf.push_back(clause);

  // VSIDS: Bump activity scores of all literals of the learned clause
  for (int lit : clause) varIncActivity(index(lit));

  // assign watched literals for the learned clause
  clause[0] > 0 ? vars[index(clause[0])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[0])].neg_watched.insert(cnf.size() - 1);

  clause[1] > 0 ? vars[index(clause[1])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[1])].neg_watched.insert(cnf.size() - 1);

  
  // The first lit is free due to non-chronological backtracking => enqueue
  if(!vars[index(clause[0])].enqueued)
    {
    unitQueue.push(Unit(clause[0], cnf.size() - 1));
    vars[index(clause[0])].enqueued = true;
  }
}

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
  int learned_half = (cnf.size() - 1 - learned_begin) / 2 ;
  printf("ANZAHL: %i\n", learned_half);
  for (int i = learned_begin; i < learned_half; i++) {
    vars[index(cnf[i][0])].pos_watched.erase(i);
    vars[index(cnf[i][1])].neg_watched.erase(i);
  }
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
    updateWatched(std::abs(literal));
  } else {
    // if (phase[index(literal)] != FREE)
    // vars[index(literal)].setValue(phase[index(literal)]);
    lit.setValue(TRUE);
    curDecisionLevel++;
    trail.push_back(literal);
    lit.level = curDecisionLevel;
    lit.reason = 0;
    decision_vars.push_back(literal);
    updateWatched(literal);
  }
}

void unassignLit(int literal) {
  int toUnassign = index(literal);
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

double var_inc = 5.0;

void createHeap() {
  act.resize(numOfVars+1);
  for (int i = 1; i <= numOfVars; i++) {
    if(i < act.size() && i < vars.size())
      act[i] = vars[i].tot_occ;
  }

  heap.createHeap();
}

void varIncActivity(int var) {
  act[var] += var_inc;
  heap.update(var);
};

void allVarsHalfActivity() {
    for (int i = 1; i <= numOfVars; i++) {
        act[i] *= 0.5;
        heap.update(i);
    }
};

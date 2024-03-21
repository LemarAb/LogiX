#include "../../include/cdcl.hpp"
#include <algorithm>
#include <cmath>
#include "dataStructs/heap.hpp"
#include "dataStructs/vsids.hpp"

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
  // learnedClauses.push_back(clause);

  clause[0] > 0 ? vars[index(clause[0])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[0])].neg_watched.insert(cnf.size() - 1);

  clause[1] > 0 ? vars[index(clause[1])].pos_watched.insert(cnf.size() - 1)
                : vars[index(clause[1])].neg_watched.insert(cnf.size() - 1);

  if(!vars[index(clause[0])].enqueued)
    {
    unitQueue.push(Unit(clause[0], cnf.size() - 1));
    vars[index(clause[0])].enqueued = true;
  }

  // for (int i = 0; i < clause.size(); i++) {
  //   vars[index(clause[i])].inlearned = true;
  // }

  // int learnedClausesSize = learnedClauses.size();
  // if(learnedClausesSize = powerOfTwo(learned2power)) {
  //   deleteHalfLearnedClauses(); // or deleteClauses()
  //   learned2power++;
  // }
}

// void deleteClauses() { // k-bounded learning
//   int k = 3;

//   for (int i = 0; i < learnedClauses.size(); i++) {
//     if (learnedClauses[i].size() > k) {
//       int unassignedCount = 0;
//       for (const auto &literal : learnedClauses[i]) {
//         if (vars[index(literal)].learned_and_unassig) {
//           unassignedCount++;
//         }
//       }
//       if (unassignedCount >= 2) {
//         for (int j = 0; i < numOfVars; i++) {
//           vars[i].pos_watched.erase(numOfClauses + i);
//           vars[i].neg_watched.erase(numOfClauses + i);
//         }
//       deletedClauses.push_back(learnedClauses[i]);
//       }
//     }
//   }
// }

// int powerOfTwo(int n) {
//     return std::pow(2, n + 2);
// }

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
    vars[index(literal)].reason = curDecisionLevel == 0 ? -1 : unitQueue.front().reason;
    unitQueue.pop(); 
    lit.level = curDecisionLevel;
    updateWatched(std::abs(literal));
  } else {
    // if (phase[index(literal)] != FREE) vars[index(literal)].setValue(phase[index(literal)]);
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
  if(vars[toUnassign].getValue() != FREE)
    {
    vars[toUnassign].setValue(FREE);
    trail.pop_back();
  } else {
    vars[literal].enqueued = false;
    unitQueue.pop();
  }
}

double var_inc = 1.0;

void createHeap() {
  act.resize(numOfVars+1);
  for (int i = 1; i <= numOfVars; i++) {
    if(i < act.size() && i < vars.size())  // Because of out of bounds access
      act[i] = vars[i].tot_occ;
    //printf("%d: %f\n", i, act[i]);
  }

  heap.createHeap();
  
//   //heap.displaySize();
  // heap.display(act);
// }
}
void varIncActivity(int var) {
    act[var] += var_inc;
    heap.update(var);
    //heap.display(act);
};

// void afterExtractOrderAct(int i) {
//     for (int j = heap.getHeapSize() / 2; j >= 0; --j) {
//         heap.update(j);
//     }
// };

// void varDecActivity(int var) {
//     act[var] -= var_inc;
//     heap.increase(var);
// };

void allVarsHalfActivity() {
    for (int i = 1; i <= numOfVars; i++) {
        act[i] *= 0.5;
        heap.update(i); // -> is this really necessary?
    }
};

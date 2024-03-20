#include "../../include/cdcl.hpp"

bool eval(int literal) {
  return !(vars[index(literal)].getValue() ^ (literal > 0));
}

int index(int literal) { return std::abs(literal); }

bool isFree(int literal) { return vars[index(literal)].getValue() == FREE; }

void addClause(std::vector<int> & clause){

  cnf.push_back(clause);

  clause[0] > 0 ? vars[index(clause[0])].pos_watched.insert(cnf.size()-1)
                      : vars[index(clause[0])].neg_watched.insert(cnf.size()-1);

  clause[1] > 0 ? vars[index(clause[1])].pos_watched.insert(cnf.size()-1)
                      : vars[index(clause[1])].neg_watched.insert(cnf.size()-1);

  if (vars[clause[0]].getValue() == FREE) {
      if (!vars[clause[0]].enqueued) {
        unitQueue.push(Unit(clause[0], cnf.size()-1));
        vars[clause[0]].enqueued = true;
      }
    } 
}


void assertLit(int literal, bool forced) {
  auto &lit = vars[std::abs(literal)];
  (literal > 0) ? lit.setValue(TRUE) : lit.setValue(FALSE);
  if (forced) {
    trail.push_back(literal);
    lit.enqueued = false;
    // assig.push(std::abs(literal));
    vars[index(literal)].reason = unitQueue.front().reason;
    unitQueue.pop(); 
    lit.level = curDecisionLevel;
    updateWatched(std::abs(literal));
  } else {
    curDecisionLevel++;
    trail.push_back(literal);
    // assig.push(literal);
    lit.level = curDecisionLevel;
    decision_vars[curDecisionLevel] = literal;
    updateWatched(literal);
  }
}

void unassignLit(int literal){
  int toUnassign = index(literal);
  vars[toUnassign].level = -1;
  vars[toUnassign].reason = -1;
  if(vars[toUnassign].getValue() != FREE)
    {
    vars[toUnassign].setValue(FREE);
    trail.pop_back();
    }
  else
    {vars[literal].enqueued = false;
      unitQueue.pop();}

}
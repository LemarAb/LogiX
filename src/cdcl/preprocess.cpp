#include "../../include/cdcl.hpp"
#include <algorithm>
#include <chrono>

// Niver Preprocessing Technique
std::vector<std::vector<int>> newcnf;
bool niverChange = true;
std::unordered_map<int, std::vector<std::vector<int>>> niverRemovedClauses;
std::vector<int> niverRemovalOrder;
std::vector<std::vector<int>> resolvents(100);

bool totalLitsinClausesComp(const std::vector<std::vector<int>> &resolvents,
                            const std::vector<int> &with,
                            const std::vector<int> &without) {
  int totalLitsinClauses = 0;
  int totalLitsinResolvents = 0;

  for (int i = 0; i < with.size(); i++) {
    totalLitsinClauses += newcnf[with[i]].size();
  }

  for (int i = 0; i < without.size(); i++) {
    totalLitsinClauses += newcnf[without[i]].size();
  }

  for (int i = 0; i < resolvents.size(); i++) {
    totalLitsinResolvents += resolvents[i].size();
  }

  if (totalLitsinResolvents <= totalLitsinClauses) {
    return true;
  } else {
    return false;
  }
}

void resolve(const std::vector<int> &with, const std::vector<int> &without,
             int var) {

  resolvents.clear();

  // No resolvents if either with or without is empty

  if (with.size() == 0 || without.size() == 0) {
    return;
  }

  std::vector<int> tmp_clause;
  tmp_clause.reserve(10);

  for (int i = 0; i < with.size(); i++) {
    for (int j = 0; j < without.size(); j++) {

      tmp_clause.clear();

      // Copy all elements from with[i] and without[i] to tmp_clause if var
      // is not present in the clause
      std::copy_if(newcnf[with[i]].begin(), newcnf[with[i]].end(),
                   std::back_inserter(tmp_clause), [var](int x) {
                     return x != var && x != -var;
                     ;
                   });

      std::copy_if(newcnf[without[j]].begin(), newcnf[without[j]].end(),
                   std::back_inserter(tmp_clause), [var, &tmp_clause](int x) {
                     return x != var && x != -var &&
                            (std::find(tmp_clause.begin(), tmp_clause.end(),
                                       x) == tmp_clause.end());
                     ;
                     ;
                   });

      // Check for tautologies in each tmp_clause, push to resolvents if not

      bool tautology = false;

      std::unordered_set<int> tmp_clause_set(tmp_clause.begin(),
                                             tmp_clause.end());

      for (int l = 0; l < tmp_clause.size(); l++) {
        if (tmp_clause_set.find(-tmp_clause[l]) != tmp_clause_set.end()) {
          tautology = true;
          break;
        }
      }
      if (!tautology) {
        resolvents.push_back(tmp_clause);
        tautology = false;
      }
    }
  }

  /* if new resolvents, check the size of the resolvents with comparison to the
     original clauses. If clause count is less or equal, alter cnf accordingly
   */
  if (resolvents.size() > with.size() + without.size()) {

    return;

  } else if (resolvents.size() <= with.size() + without.size()) {

    std::vector<int> toRemove; // clauses to be removed, only the index
    toRemove.insert(toRemove.end(), with.begin(), with.end());
    toRemove.insert(toRemove.end(), without.begin(), without.end());
    std::sort(toRemove.begin(), toRemove.end()); // remove in reverse order

    for (int i = toRemove.size() - 1; i >= 0; i--) {
      niverRemovedClauses[var].push_back(
          newcnf[toRemove[i]]); // store removed clauses to add back later
      newcnf.erase(newcnf.begin() + toRemove[i]);
    }
    niverRemovalOrder.push_back(var);

    // add resolvents to cnf
    for (int i = 0; i < resolvents.size(); i++) {
      newcnf.push_back(resolvents[i]);
    }
  }
}

void clauseDecomp(int var) {
  std::vector<int> withVarPos;
  std::vector<int> withVarNeg;
  std::vector<int> remaining;

  withVarPos.reserve(100);
  withVarNeg.reserve(100);
  remaining.reserve(200);

  for (int i = 1; i < newcnf.size(); i++) {
    if (find(newcnf[i].begin(), newcnf[i].end(), var) != newcnf[i].end()) {
      withVarPos.push_back(i);
    } else if (find(newcnf[i].begin(), newcnf[i].end(), -var) !=
               newcnf[i].end()) {
      withVarNeg.push_back(i);
    } else {
      remaining.push_back(i);
    }
  }

  resolve(withVarPos, withVarNeg, var);
}

bool literalsinCNFs(const std::vector<std::vector<int>> &newcnf,
                    const std::vector<std::vector<int>> &cnf) {

  // Preliminary check for sizes
  if (newcnf.size() > cnf.size()) {
    return false;
  }
  int totalLitsinCNF = 0;
  int totalLitsinNewCNF = 0;

  for (int i = 0; i < cnf.size(); i++) {
    totalLitsinCNF += cnf[i].size();
  }

  for (int i = 0; i < newcnf.size(); i++) {
    totalLitsinNewCNF += newcnf[i].size();
  }

  if (totalLitsinNewCNF <= totalLitsinCNF) {
    return true;
  } else {
    return false;
  }
}

void checkChangeinCNF(const std::vector<std::vector<int>> &newcnf) {

  if (newcnf.size() < cnf.size()) {
    cnf = newcnf;
    niverChange = true;
  } else if (newcnf.size() == cnf.size()) {
    if (literalsinCNFs(newcnf, cnf) && newcnf != cnf) {
      cnf = newcnf;
      niverChange = true;
    }
  } else {
    niverChange = false;
  }
}

int niver() {

  for (auto &vec : resolvents) {
    vec.reserve(100);
  }

  newcnf = cnf;

  while (niverChange) {
    niverChange = false;
    for (int i = 1; i <= numOfVars; i++) {
      clauseDecomp(i);
    }
    checkChangeinCNF(newcnf);
  }

  if (niverRemovalOrder.size() == 0) {
    return -1;
  }
  numOfClauses = cnf.size() - 1;

  return 0;
}

void removeDuplicateClauses() {
  std::set<std::vector<int>> uniqueClauses(cnf.begin(), cnf.end());
  cnf.assign(uniqueClauses.begin(), uniqueClauses.end());

  numOfClauses = cnf.size() - 1;
}

void refreshInfosafterPrep() {
  for (int i = 1; i <= numOfVars; i++) {
    vars[i].pos_watched.clear();
    vars[i].neg_watched.clear();
    vars[i].enqueued = false;
  }

  int afterPrepnumOfVars = 0;
  for (const auto &clause : cnf) {
    afterPrepnumOfVars += clause.size();
  }

  Assig *unit = new Assig[afterPrepnumOfVars];
  for (int i = 1; i < cnf.size(); i++) {
    if (cnf[i].size() == 1) {

      if (!vars[index(cnf[i][0])].enqueued) {
        unit[index(cnf[i][0])] = Assig(cnf[i][0] > 0);
        unitQueue.push(Unit(cnf[i][0], -1));
        vars[index(cnf[i][0])].enqueued = true;
        unitTrail.push_back(cnf[i][0]);
        countu++;
      }

      else if (unit[index(cnf[i][0])] != Assig((cnf[i][0] > 0)))
        return;
    }

    if (cnf[i].size() > 1) {

      cnf[i][0] > 0 ? vars[index(cnf[i][0])].pos_watched.insert(i)
                    : vars[index(cnf[i][0])].neg_watched.insert(i);

      cnf[i][1] > 0 ? vars[index(cnf[i][1])].pos_watched.insert(i)
                    : vars[index(cnf[i][1])].neg_watched.insert(i);
    }
  }
}

int preprocess() {
  int removedSomething = niver();
  if (removedSomething != 0) {
    return -1;
  }
  removeDuplicateClauses();
  refreshInfosafterPrep();
  return 0;
}
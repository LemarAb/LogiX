#include "../include/cnf.hpp"

void revert(int unassignedVar) {
    // clauses where assertedVar evaluates to FALSE
    std::set<int>* clausesToIncrement;

    // all clauses the unassignedVar appears in (including satisfied clauses) and evaluates to TRUE
    std::set<int>* allOccurences;

    allOccurences = (vars[unassignedVar].val == TRUE) ? &vars[unassignedVar].static_pos_occ : &vars[unassignedVar].static_neg_occ;

    if (vars[unassignedVar].val == TRUE) {
        clausesToIncrement = &vars[unassignedVar].static_neg_occ;
    } else {
        clausesToIncrement = &vars[unassignedVar].static_pos_occ;
        unassignedVar = -unassignedVar;
    }

    std::set<int> copy2 = *clausesToIncrement;

    for (auto clauseIndex2 = copy2.begin(); clauseIndex2 != copy2.end(); ++clauseIndex2) clauses[*clauseIndex2].active++;


    std::set<int> copy = *allOccurences;

    // restore the previously deleted clauses and its var references
    for (auto clauseIndex = copy.begin(); clauseIndex != copy.end(); ++clauseIndex) {
        if (clauses[*clauseIndex].sat != unassignedVar) continue;

        Clause* clause = &clauses[*clauseIndex];

        clause->sat = 0;

        for (int i = 0; i < clause->literals.size(); i++) {
            clause->literals[i] > 0 ? vars[std::abs(clause->literals[i])].pos_occ.insert(*clauseIndex)
                                    : vars[std::abs(clause->literals[i])].neg_occ.insert(*clauseIndex);
        }
        numOfSatClauses--;
    }
}
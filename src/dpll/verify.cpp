#include "../../include/cnf.hpp"

void verifyModel() {
    for (int i = 1; i <= numOfClauses; i++) {
        bool sat = false;
        
        for (int literal : clauses[i].literals) {
            if (evaluateLiteral(literal)) sat = true;
        }
        if (!sat) {
            printf("Error at %i ", i);
            break;
        }
        if (i == numOfClauses) printf("\n\033[32mMODEL VERIFIED!\033[0m\n");
    }
}

bool evaluateLiteral(int literal) {
    return vars[std::abs(literal)].val == FREE || !(vars[std::abs(literal)].val ^ (literal > 0));
}
#include "../../include/cnf2.hpp"

void verifyModel() {
    for (int i = 1; i <= numOfClauses; i++) {
        bool sat = false;
        for (int literal : cnf[i]) {
            if (eval(literal)) sat = true;
        }
        if (!sat) {
            printf("Error at %i ", i);
            break;
        }
        if (i == numOfClauses) printf("MODEL VERIFIED!\n");
    }
}
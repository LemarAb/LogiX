#include "../../include/cnf2.hpp"

void printModel(int res) {
    if (res == 1) {
        printf("\nUNSAT: No model!\n");
    } else {
        printf("Model: ");
    }

    printf("[");
    for (int i = 1; i < numOfVars; i++) {
        int val;
        if (vars[i].getValue() == FREE) val = 0;
        if (vars[i].getValue() == TRUE) val = i;
        if (vars[i].getValue() == FALSE) val = -i;

        printf("%i, ", val);
    }
    int val;
    if (vars[numOfVars].getValue() == FREE) val = 0;
    if (vars[numOfVars].getValue() == TRUE) val = numOfVars;
    if (vars[numOfVars].getValue() == FALSE) val = -numOfVars;
    printf("%i", val);
    printf("]\n");
}
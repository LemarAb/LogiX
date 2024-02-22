#include "../../include/cnf.hpp"

void printModel(int res) {
    if (res == 1) {
        printf("\033[31mUNSAT: No model!\n\n[");
        for (int i = 1; i <= numOfVars; i++) {
            int value = 0;
            if (vars[i].val == FREE) value = 0;
            if (vars[i].val == TRUE) value = i;
            if (vars[i].val == FALSE) value = -i;

            printf("%i", value);
            (i < numOfVars) ? printf(", ", value) : printf("]\033[0m\n");
            if (i % 20 == 0) printf("\n");
        }
    } else {
        std::cout << "\033[1;32mSAT!";
        printf(" Model:\n\n[");
        for (int i = 1; i <= numOfVars; i++) {
            int value = 0;
            if (vars[i].val == FREE) value = -i;
            if (vars[i].val == TRUE) value = i;
            if (vars[i].val == FALSE) value = -i;

            printf("%i", value);
            (i < numOfVars) ? printf(", ", value) : printf("]\033[0m\n");
            if (i % 20 == 0) printf("\n");
        }
    }
}


void writeModelToFile(int res, const std::string& fileName) {
    std::ofstream outFile("models/" + fileName);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    if (res == 1) {
        outFile << "-s UNSATISFIABLE: No model";
    } else {
        outFile << "-s SATISFIABLE!";
        outFile << "\n\n";
        outFile << "v- ";
        for (int i = 1; i <= numOfVars; i++) {
            int value = 0;
            if (vars[i].val == FREE) value = -i;
            if (vars[i].val == TRUE) value = i;
            if (vars[i].val == FALSE) value = -i;

            outFile << value;

            (i < numOfVars) ? outFile << ", " : outFile << "\n";
            if (i % 20 == 0) outFile << "\nv- ";
        }
    }

    outFile.close();
}
// Niver Preprocessing Technique

#include "include/cdcl.hpp"
#include <algorithm>

int numOfVars;
int numOfClauses;
int numOfUnassigned;
std::vector<std::vector<int>> cnf;
std::vector<Variable> vars;
std::set<int> satClauses;
std::queue<int> unitQueue;

void clauseDecomp(int var) {
    std::vector<int> withVarPos;
    std::vector<int> withVarNeg;
    std::vector<int> remaining;

    for (int i = 1; i <= numOfClauses; i++) {
        if (find(cnf[i].begin(), cnf[i].end(), var) != cnf[i].end()) {
            withVarPos.push_back(i);
        } else if (find(cnf[i].begin(), cnf[i].end(), -var) != cnf[i].end()) {
            withVarNeg.push_back(i);
        } else {
            remaining.push_back(i);
        }
    }

    std::cout << "Var= " << var << std::endl;

    // Print all elements of withVar
    std::cout << "withVar: ";
    for (int i = 0; i < withVarPos.size(); i++) {
        std::cout << withVarPos[i] << " ";
    }
    std::cout << std::endl;

    // Print all elements of woutVar
    std::cout << "woutVar: ";
    for (int i = 0; i < withVarNeg.size(); i++) {
        std::cout << withVarNeg[i] << " ";
    }
    std::cout << std::endl;

    // Print all elements of remaining
    std::cout << "remaining: ";
    for (int i = 0; i < remaining.size(); i++) {
        std::cout << remaining[i] << " ";
    }
    std::cout << std::endl;

    printf("\n");

}

int main() {

  std::string filename = "benchmarks/test/001_count4_2_s.cnf";

  parseDIMACS(filename);

  int n = numOfVars;
  printf("Number of Variables: %i\n", n);

  for (int i = 1; i <= n; i++) {
    clauseDecomp(i);
  }
  return 0;
}
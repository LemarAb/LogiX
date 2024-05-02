#include <unistd.h>

#include <chrono>
#include <iostream>

#include "../../include/cdcl.hpp"
#include "../../include/fileNames.hpp"

int numOfVars;
int numOfClauses;
int numOfUnassigned;
std::vector<std::vector<int>> cnf;
std::vector<Variable> vars;
std::set<int> satClauses;
std::queue<Unit> unitQueue;
std::stack<int> assig;
int curVar = 1;
int curProp;
Heuristics heuristic = INC;
std::vector<int> decision_vars;
std::vector<Assig> phase;
bool prepr = false;
bool proof = false;
std::vector<int> learnedUnits;

int learned_begin;
int main(int argc, char *argv[]) {
  // measure CPU time...
  std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();

  std::string path = argv[1];

  std::string index;

  for (int i = 1; i < path.length(); i++)
    index += path[i];  

  std::string fileName;

  if (path[0] == 't')
    fileName = "benchmarks/test/" + fileNamesTest[std::stoi(index)];

  if (path[0] == 'c')
    fileName = "benchmarks/comp/" + fileNamesComp[std::stoi(index)];

  printf("\nRunning %s\n\n", fileName.c_str());

  bool sat;

  // if we find a conflict upon parsing the DIMACS, skip the solver
  if (!parseDIMACS(fileName))
    sat = false;
  else
    sat = cdcl();

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

  printModel(sat);
  
  std::chrono::duration<double> duration =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

  if (sat)
    verifyModel();
  else {
    auto logPath = proofLog(fileName);
    std::string exec = "drat-trim " + fileName + " " + logPath;
    system(exec.c_str());
  }

  printf("\nCPU time used: %.6f seconds\n\n", duration.count());

  std::cout.flush();

  printf("-------------------------------------\n\n", duration.count());

  return 0;
}

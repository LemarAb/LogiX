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
std::queue<int> unitQueue;
std::stack<int> assig;
std::vector<int> vsids;
int curVar = 1;
int curProp;
Heuristics heuristic = INC;

int main(int argc, char *argv[]) {
  // measure CPU time...
  std::chrono::steady_clock::time_point start =
      std::chrono::steady_clock::now();

  std::string path = argv[1];

  std::string index;

  for (int i = 1; i < path.length(); i++) {
    index += path[i];
  }

  std::string fileName;

  if (path[0] == 't')
    fileName = "benchmarks/test/" + fileNamesTest[std::stoi(index)];

  if (path[0] == 'c')
    fileName = "benchmarks/comp/" + fileNamesComp[std::stoi(index)];

  printf("\nRunning %s\n\n", fileName.c_str());

  //   if (argc > 2)
  //     heuristic = Heuristics(std::stoi(argv[2]));

  parseDIMACS(fileName);

  pthread_t thread;

  if (pthread_create(&thread, NULL, dpll, NULL)) {
    std::cerr << "Error: Unable to create thread." << std::endl;
    return -1;
  }
  // Wait for the child thread to finish
  void *res;
  pthread_join(thread, &res);

  printModel((intptr_t)res);

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

  if ((intptr_t)res == 0)
    verifyModel();

  printf("\nCPU time used: %.6f seconds\n\n", duration.count());
  // for (auto it = satClauses.begin(); it != satClauses.end(); ++it) {
  //     std::cout << *it << std::endl;  // Perform operations with each element
  // }

  // std::cout << "\nCPU time used: " << duration.count() << " seconds\n" <<
  // std::endl;
  std::cout.flush();

  printf("-------------------------------------\n\n", duration.count());

  return 0;
}

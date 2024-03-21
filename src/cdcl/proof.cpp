#include "../../include/cdcl.hpp"
#include <vector>

std::vector<std::vector<int>> learnedClauses;
std::vector<std::vector<int>> deletedClauses;

void proofLogging(std::string filename) {

  //for (int i = 0; i < 15; i++) {
    filename.erase(0, 16);
    printf ("Filename: %s\n", filename.c_str());
  //}

  for (int i = 0; i < 3; i++) {
    filename.pop_back();
  }

  filename += "drup";

  std::string path = "proofs/";  
  std::ofstream out(path + filename);
  auto *oldCoutBuffer = std::cout.rdbuf(); // Save old buf
  std::cout.rdbuf(out.rdbuf());            // Redirect std::cout to new .drup

  // Your code here. Anything printed to std::cout will go to output.txt.

  for (int i = 0; i < learnedClauses.size(); i++) {
    std::cout << "  ";
    for (int j = 0; j < learnedClauses[i].size(); j++) {
      std::cout << learnedClauses[i][j] << " ";
    }
    std::cout << "0\n";
  }

  for (int i = 0; i < deletedClauses.size(); i++) {
    std::cout << "d ";
    for (int j = 0; j < deletedClauses[i].size(); j++) {
      std::cout << deletedClauses[i][j] << " ";
    }
    std::cout << "0\n";
  }

  std::cout.rdbuf(oldCoutBuffer);
}

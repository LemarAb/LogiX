#include "../../include/cdcl.hpp"
#include <vector>

std::vector<std::vector<int>> learnedClauses;
std::vector<std::vector<int>> deletedClauses;

std::string proofLog(std::string filename) {

  filename.erase(0, 16);

  for (int i = 0; i < 3; i++) {
    filename.pop_back();
  }

  filename += "drup";

  std::string path = "proofs/"+ filename;
  std::ofstream out(path);
  auto *oldCoutBuffer = std::cout.rdbuf(); // Save old buf
  std::cout.rdbuf(out.rdbuf());            // Redirect std::cout to new .drup

  for (int i = learned_begin; i < cnf.size(); i++) {
    std::cout << "  ";
    for (int j = 0; j < cnf[i].size(); j++) {
      std::cout << cnf[i][j] << " ";
    }
    std::cout << "0\n";
  }

  for (int i = 0; i < learnedUnits.size(); i++){
    std::cout << learnedUnits[i] << " " << 0 << "\n";
  }

  /* for (int i = 0; i < deletedClauses.size(); i++) {
    std::cout << "d ";
    for (int j = 0; j < deletedClauses[i].size(); j++) {
      std::cout << deletedClauses[i][j] << " ";
    }
    std::cout << "0\n";
  } */

  std::cout.rdbuf(oldCoutBuffer);

  return path;
}

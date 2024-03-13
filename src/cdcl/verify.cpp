#include "../../include/cdcl.hpp"

void verifyModel() {
  for (int i = 1; i < OGcnf.size(); i++) {
    bool sat = false;
    for (int literal : OGcnf[i]) {
      if (eval(literal) || vars[index(literal)].getValue() == FREE)
        sat = true;
    }
    if (!sat) {
      printf("Error at %i ", i);
      break;
    }
    if (i == OGcnf.size() - 1)
      printf("\n\033[32mMODEL VERIFIED!\033[0m\n");
  }
}
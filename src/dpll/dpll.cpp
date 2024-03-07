#include "../../include/cnf.hpp"

void* cdcl(void* arg) {
    while (true) {
        propagate();
        decide();
    }
}
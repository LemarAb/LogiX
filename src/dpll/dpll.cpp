#include "../../include/cnf.hpp"

void* dpll(void* arg) {
    while (true) {
        propagate();
        decide();
    }
}
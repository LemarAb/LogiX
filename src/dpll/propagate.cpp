#include "../../include/cnf.hpp"

void propagate() {
    int prop;
    while (!toPropagate.empty()) {
        prop = toPropagate.front();
        toPropagate.pop();
        int absProp = std::abs(prop);
        
        (prop > 0) ? vars[absProp].val = TRUE : vars[absProp].val = FALSE;
        vars[absProp].enqueued = false;
        vars[absProp].forced = true;

        assig.push(absProp);
        
        update(absProp);
    }
}
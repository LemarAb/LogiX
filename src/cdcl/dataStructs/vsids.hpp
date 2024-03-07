#include "intmap.hpp"
#include "vector.hpp"
#include "heap.hpp"


#ifndef VSIDS_HPP
#define VSIDS_HPP

IntMap<int, double> act;

VariableOrderActLT lt(act);

Heap<int, VariableOrderActLT> vsidsheap(lt); 

double var_inc = 5.0;
    
void varIncActivity(int var) {
    act[var] += var_inc;
    vsidsheap.decrease(var);
};

void afterExtractOrderAct(int i) {

    act[i] = -1;
    //for (int j = i; j < vsidsheap.getHeapSize() - 1; j++) {
    //    act[j] = act[j + 1];
    //}

    for (int j = vsidsheap.getHeapSize() / 2; j >= 0; --j) {
        vsidsheap.update(j);
    }
};

void varDecActivity(int var) {
    act[var] -= var_inc;
    vsidsheap.increase(var);
};

void allVarsHalfActivity() {
    for (int i = 1; i <= numOfVars; i++) {
        act[i] *= 0.5;
        vsidsheap.update(i);
    }
};

#endif
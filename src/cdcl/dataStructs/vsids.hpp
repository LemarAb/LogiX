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

#endif
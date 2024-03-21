#include "heap.hpp"


#ifndef VSIDS_HPP
#define VSIDS_HPP

extern std::vector<double> act;

extern VariableOrderActLT lt;

extern Heap<int, VariableOrderActLT> heap;
    
extern void varIncActivity(int var);

extern void afterExtractOrderAct(int i);

extern void varDecActivity(int var);

extern void allVarsHalfActivity();

#endif

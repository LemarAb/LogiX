#include "heap.hpp"

#ifndef VSIDS_HPP
#define VSIDS_HPP

// Stores the activity of every variable
extern std::vector<double> act;

extern VariableOrderActLT lt;

// max heap
extern Heap<int, VariableOrderActLT> heap;

// bump activity of learned clause literals
extern void varIncActivity(int var);

// half activity scores of all vars every 255th decision
extern void allVarsHalfActivity();

#endif

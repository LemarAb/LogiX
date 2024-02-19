#include <math.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef MYHEADER_HPP
#define MYHEADER_HPP

extern int numOfVars;
extern int numOfClauses;

enum Heuristics { INC, DLIS, DLCS, JW };

enum Assig {
    FALSE,
    TRUE,
    FREE,
};

struct Variable {
    Assig val = FREE;

    std::set<int> static_pos_occ;  // All clauses where var appears as pos literal
    std::set<int> static_neg_occ;  // All clauses where var appears as neg literal

    std::set<int> pos_occ;  // All unsatisfied clauses where var appears as pos literal
    std::set<int> neg_occ;  // All unsatisfied clauses where var appears as neg literal

    bool forced = false;
    bool enqueued = false;
};

struct Clause {
    std::vector<int> literals;
    int active;
    int sat = 0;
};

extern Heuristics heuristic;

// the currently set branching variable
extern int curVar;

extern int numOfSatClauses;

// decision counter
extern int dc;

// backtrack counter
extern int btc;

// flag to determine whether to backtrack or not
extern bool backtrackFlag;

// vec of clauses (1-indexed)
extern std::vector<Clause> clauses;

// vec of variables (1-indexed)
extern std::vector<Variable> vars;

// queue storing unit and pure literals to be propagated
extern std::queue<int> toPropagate;

// stack of variables with assigned values
extern std::stack<int> assig;

void parseDIMACS(std::string filename);

void* dpll(void* arg);

// asserts pure and unit literals
void propagate();

// chooses next branching var according to the selected heuristic
extern void (*decide)();

void chooseINC();

void chooseDLIS();

void chooseDLCS();

void chooseJW();

// updates the CNF after a new assignment is made;
// exits dpll if valid assig is found
void update(int assertedVar);

// updates the CNF after unassignment in backtrack()
void revert(int unassignedVar);

// handles conficts and signals UNSAT
void backtrack();

// evaluates the literal under its current assignment
bool evaluateLiteral(int literal);

void printModel(int res);

void verifyModel();

// print dimacs filees
void writeModelToFile(int res, const std::string& fileName);

#endif

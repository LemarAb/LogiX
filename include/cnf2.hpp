#include <stdio.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

#ifndef MYHEADER_HPP
#define MYHEADER_HPP

extern int numOfVars;
extern int numOfClauses;
// num of not yet assigned variables;
extern int numOfUnassigned;

enum Heuristics { INC, DLIS, DLCS, MOM, JW };

enum Assig {
    FALSE,
    TRUE,
    FREE,
};

enum Polarity { NEG, POS, MIX, UNSET };

struct Variable {
   private:
    Assig val = FREE;

   public:
    // std::set<int> pos_pol;  // = {1,2};  All clauses where var appears as pos watched literal
    //     std::set<int>
    //         neg_poll;  // = {3,4} All clauses where var appears as neg watched literal
    //             //    (1 2 -3) (1 -2 3 4) (-1 2 -4) (-1 3 -4)
    //             // clause x sat => x is in neg_poll => erase x from neg_poll
    //             // if neg_pol.empty() => pureLiter => set var to 1

    std::set<int> pos_watched;  // All clauses where var appears as pos watched literal
    std::set<int> neg_watched;  // All clauses where var appears as neg watched literal
    bool forced = false;
    int pos_occ;  // number of clauses var appears as pos literal
    int neg_occ;  // number of clauses var appears as neg literal
    bool enqueued = false;
    void setValue(Assig _assig) {
        // int assertedLit = unitProp ? curProp : curVar;
        if (_assig != FREE && val == FREE)
            numOfUnassigned--;
        else {
            if (_assig == FREE) numOfUnassigned++;

            // else
            //     vars[assertedLit].forced = true;
        }
        val = _assig;
        // printf("num of unassigned: %i \n", numOfUnassigned);

        // vars[assertedLit].enqueued = false;
        // vars[assertedLit].forced = true;
        // assig.push(assertedLit);
        // updateWatchedLiterals(assertedLit);
    }
    Assig getValue() { return val; }
};

struct Clause {
    std::vector<int> literals;
    int w1 = 0;
    int w2 = 1;
    int sat = -1;
};

extern Heuristics heuristic;

// the currently processed variable
extern int curVar;

// the currently processed unit literal
extern int curProp;

// list of clauses (1-indexed)
extern std::vector<std::vector<int>> cnf;

// list of variables (1-indexed)
extern std::vector<Variable> vars;

// set of unsatisfied clauses
extern std::set<int> satClauses;

// queue storing unit literals
extern std::queue<int> unitQueue;

// stack of variables with assigned values
extern std::stack<int> assig;

// set of variables occuring only in negative polarity
extern std::unordered_set<int> neg_pol;

// set of variables occuring only in positive polarity
extern std::unordered_set<int> pos_pol;

void parseDIMACS(std::string filename);

void* dpll(void* arg);

void unitPropagate();

// chooses literals according to the used heuristic
void chooseLiteral();

void chooseINC();

void chooseDLIS();

void chooseDLCS();

void chooseMOM();

void chooseJW();

// updates the watched literals after a new assignment is made
void updateWatchedLiterals(int literal);

// handles conficts and signals UNSAT
void backtrack();

// evaluates the literal under its current assignment
bool eval(int literal);

void printModel(int res);

void verifyModel();

#endif
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

extern std::vector<int> decision_vars;

extern int countu;
enum Polarity { NEG, POS, MIX, UNSET };

extern int decision_count;

struct Variable {
   private:
    Assig val = FREE;

   public:
    int level = -1;
    int reason = 0;
    std::set<int> pos_watched;  // All clauses where var appears as pos watched literal
    std::set<int> neg_watched;  // All clauses where var appears as neg watched literal
    int pos_occ = 0;  // number of clauses var appears as pos literal
    int neg_occ = 0;  // number of clauses var appears as neg literal
    int tot_occ = 0;  // total number of clauses var appears in
    bool enqueued = false;
    void setValue(Assig _assig) {
        val = _assig;
    }
    Assig getValue() { return val; }
};

struct Unit {
    int literal;
    int reason = 0;

    Unit(int _literal, int _reason):
    literal(_literal), reason(_reason){
      // printf("REASON CONSTR: %i\n", reason);
    }
};

extern std::vector<int> unitTrail;

extern std::vector<std::vector<int>> OGcnf;

extern Heuristics heuristic;

// the currently processed variable
extern int curVar;

// the currently processed unit literal
extern int curProp;

// list of clauses (1-indexed)
extern std::vector<std::vector<int>> cnf;

// list of variables (1-indexed)
extern std::vector<Variable> vars;

extern int learned_begin;

extern double delete_cue;

// forget half of the learned claueses every 
void delete_half();

// queue storing unit literals
extern std::queue<Unit> unitQueue;

// stack of variables with assigned values
extern std::stack<int> assig;

extern std::queue<int> del_ref;
// set of variables occuring only in negative polarity
extern std::unordered_set<int> neg_pol;

// set of variables occuring only in positive polarity
extern std::unordered_set<int> pos_pol;

extern std::unordered_map<int, std::vector<std::vector<int>>> niverRemovedClauses;

extern std::vector<int> niverRemovalOrder;

extern std::vector<int> trail;

extern std::vector<int> seen;

extern int curDecisionLevel;
// conflict counter 
extern int conflict_count;

extern std::vector<Assig> phase;

void createHeap();

void unassignLit(int literal);

bool parseDIMACS(std::string filename);

bool cdcl();

void unitPropagate();

// chooses literals according to the used heuristic
int pickDecisionLit();

void pickDecisionLit2();

// updates the watched literals after a new assignment is made
std::set<int>* updateWatched(int literal);

// handles conficts and signals UNSAT
void backtrack(int btlvl);

void backtrack2();

// rstart according to luby sequence
void restart();

// evaluates the literal under its current assignment
bool eval(int literal);

// add the learned clause
void addClause(std::vector<int> & clause);

int index(int literal);

void printModel(int res);

// delivers the backtrack_lvl and learns the clause
int analyze();

void verifyModel();

int preprocess();

void assertLit(int literal, bool forced);

void emptyUnitQueue();

int niverPostprocess();

std::string proofLog(std::string filename);

// find the luby sequence
int luby(int i);

extern int luby_index;

extern int luby_unit;

extern bool prepr; // bool for preprocessing

extern bool proof; // bool for proof logging

extern std::vector<int> learnedUnits; // Vector for learned units to add to log

#endif
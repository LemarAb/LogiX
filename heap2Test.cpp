
#include "src/cdcl/dataStructs/vsids.hpp"
#include "include/cdcl.hpp"
#include <cassert> 

int numOfVars;
int numOfClauses;
int numOfUnassigned;
std::vector<std::vector<int>> cnf;
std::vector<Variable> vars;
std::set<int> satClauses;
std::queue<int> unitQueue;
int conflict_count = 0;

//unfinished restart function
void restart () {
  //trail.clear();
  //vsidsheap.clear();
}


int fix_no_of_conflicts = 600;

void fixed_restart () {
  if (conflict_count == fix_no_of_conflicts) {
    restart();
  }
}

int geom_conf_lim = 150;

double geom_factor = 1.5;

void geom_restart() {
  if (conflict_count == geom_conf_lim) {
    geom_conf_lim *= geom_factor;
    restart();
  }
}

int luby_index = 1;

int luby_unit = 32;

int luby(int i) {
    int k;
    for (k = 0; (1 << (k + 1) <= (i + 1)); k++);
    if ((1 << k) == (i + 1)) return (1 << (k - 1));
    else return luby((i + 1) - (1 << k));
}

void luby_restart() {
    if (conflict_count == luby(luby_index) * luby_unit) {
        luby_index++;
        restart();
    }
}


int main() {
    // Example usage

    std::string filename = "benchmarks/comp/001_aim-100-1_6-no-1_u.cnf";

    parseDIMACS(filename);
    
    int n = numOfVars;
    printf("Number of Variables: %i\n", n);

    for (int i = 1; i <= n; i++) {
        act.insert(i, vars[i].tot_occ);
        printf("%d: %f\n", i, act[i]);
    }

    for (int i = 1; i <= n; i++) {
        vsidsheap.insert(i);
    }

    vsidsheap.display(act);

    vsidsheap.displayIndices();
    
    int test = vsidsheap.removeMax();
    printf("%d\n", test);

    
    vsidsheap.displayIndices();

    vsidsheap.display(act);

    vsidsheap.displayIndEntry(12);

    vsidsheap.displaySize();

    varIncActivity(12);

    vsidsheap.display(act);

    varDecActivity(12);

    vsidsheap.display(act);

    allVarsHalfActivity();

    vsidsheap.display(act);

    vsidsheap.remove(15);

    vsidsheap.display(act);

    vsidsheap.insert(15);

    vsidsheap.display(act);

    for (int i = 1; i <= 64; i++) {
        printf("%d ", luby(i));
        luby_index++;
    }
    printf("\n");
    return 0;
}
// **************************************************************************************************/

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


int main() {
    // Example usage

    std::string filename = "benchmarks/test/025_tent4_4_s.cnf";

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
    printf("\n");
    return 0;
}
// **************************************************************************************************/
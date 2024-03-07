
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

    for (int i = 1; i < n; i++) {
        vsidsheap.insert(i);
    }

    vsidsheap.display();

    
    int test = vsidsheap.removeMin();
    printf("%d\n", test);

    
    vsidsheap.displayIndices();

    vsidsheap.display();

    vsidsheap.displayIndEntry(12);

    printf("checking if assertion is true: %i  \n", vsidsheap.inHeap(12));
    assert(vsidsheap.inHeap(12));
    printf("true\n");

    vsidsheap.displaySize();

    varIncActivity(12);

    printf("\n");
    return 0;
}
// **************************************************************************************************/
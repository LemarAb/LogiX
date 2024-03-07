
#include "src/cdcl/dataStructs/vsids.hpp"
#include "include/cdcl.hpp"

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

    IntMap<int, double> act;

    VariableOrderActLT lt(act);

    for (int i = 1; i < n; i++) {
        act.insert(i, vars[i].tot_occ);
        printf("%d: %f\n", i, act[i]);
    }

    Heap<int, VariableOrderActLT> vsidsheap(lt); 

    for (int i = 1; i < n; i++) {
        vsidsheap.insert(i);
    }

    vsidsheap.display();

    
    int test = vsidsheap.removeMin();
    printf("%d\n", test);

    vsidsheap.display();

    varIncActivity(12);

    

    printf("\n");
    return 0;
}
// **************************************************************************************************/
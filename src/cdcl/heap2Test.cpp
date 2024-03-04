#include "heap.hpp"
#include "vector.hpp"
#include "../../include/cdcl.hpp"

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
    
    int N = vars.size();

    IntMap<int, double> act;

    VariableOrderActLT lt(act);

    for (int i = 1; i < N; i++) {
        act.insert(i, vars[i].tot_occ);
        printf("%d: %f\n", i, act[i]);
    }

    Heap<int, VariableOrderActLT> vsidsheap(lt); 

    for (int i = 1; i < N; i++) {
        vsidsheap.insert(i);
    }

    vsidsheap.display();

    
    int test = vsidsheap.removeMin();
    printf("%d\n", test);

    vsidsheap.display();

    vsidsheap.varIncActivity(12);

    printf("\n");
    return 0;
}
// **************************************************************************************************/
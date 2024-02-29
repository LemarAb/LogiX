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

    Heap<int, VariableOrderActLT> vsidsheap(lt); 

    for (int i = 1; i <= N; i++) {
        act.insert(i, vars[i].pos_occ + vars[i].neg_occ);
        vsidsheap.insert(i);
    }

    
    int test = vsidsheap.removeMin();
    printf("%d\n", test);

    printf("\n");
    return 0;
}
// **************************************************************************************************/
#include "heap.hpp"
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

    Heap vsidsheap(N);

    for (int i = 1; i < vars.size(); i++) {
    
        vsidsheap.insertKey(i);
        vsidsheap.display();
            printf("act %i \n", vars[i].pos_occ + vars[i].neg_occ);


    printf("\n");
    }

    int heapsize = vsidsheap.getHeapSize();
    printf("This is the size after init %i \n", vsidsheap.getHeapSize());

    int max = vsidsheap.popMax();
    printf("This is the max %i\n", max);
    vsidsheap.display();
    return 0;
}

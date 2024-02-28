#include "heap.hpp"
#include "../../include/cnf2.hpp"

int numOfVars;
int numOfClauses;
int numOfUnassigned;
std::vector<std::vector<int>> cnf;
std::vector<Variable> vars;
std::set<int> satClauses;
std::queue<int> unitQueue;

int main() {
    // Example usage

    std::string filename = "benchmarks/test/005_hole2_u.cnf";

    parseDIMACS(filename);
    
    int N = vars.size();

    Heap heap(N);

    heap.initHeap(heap);
    heap.display();

    //int min = heap.extractMin();
    //heap.display();
    return 0;
}

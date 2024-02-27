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

    printf("test");

    std::string filename = "benchmarks/test/005_hole2_u.cnf";

    parseDIMACS(filename);
    
    int N = vars.size();

    Heap heap(N);

    for (int i=1; i <= numOfVars; i++){
            std::cout << "instering element " << i + 1 << "\n";
            heap.insertKey(i);
            heap.display();
            std::cout << std::endl;
    }

    int min = heap.extractMin();
    heap.display();
    
    
    /*std::vector<int> formula; 

    formula[1] = 1;
    formula[2] = 3;
    formula[3] = 2;
    formula[4] = 1;
    formula[5] = 1;
    formula[6] = 4;

    Heap vsidsHeap(formula.size());

    for (int i = 1; i <= formula.size(); i++){
        vsidsHeap.insertKey(formula[i]);
    }

    std::cout << "Heap size: " << vsidsHeap.getHeapSize() << "\n";
    std::cout << "Heap contents: ";
    vsidsHeap.display();

    int maxScoreVariable = vsidsHeap.deleteKey();
    std::cout << "Variable with the highest score: " << maxScoreVariable << "\n";

    std::cout << "Updated Heap size: " << vsidsHeap.getHeapSize() << "\n";
    std::cout << "Updated Heap contents: ";
    vsidsHeap.display(); */

    return 0;
}

#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <unordered_map>

#ifndef K_Heap_hpp
#define K_Heap_hpp

// A MinHeap for VSIDS Branching Heuristic

template <class T>
class Heap{ 
    std::vector<T> heap;    
    double score;  
    double decay;

    bool scoreOperator()(int v1, int v2) const {
        
        return vsidsInfo[v1].score < vsidsInfo[v2].score;
    }

    void heapifyUp(int);
    void heapifyDown(int);

     
public: 

    Heap(){}

    int getLChild (int i) const {return i*2+1;}
    int getRChild (int i) const {return (i+1)*2;}
    int getParent (int i) const {return (i-1) / 2;}

    void insertKey(T);
    int getHeapSize() const;
    void display() const;
    T deleteKey();    


};

template<class T>
void Heap<T>::heapifyUp(int c) {

    int tmp = heap[c];
    int p = getParent(c);

    while (c != 0 && scoreOperator(tmp, heap[p])){
        heap[c] = heap[p];

    }
    
}


// Unfinished
void InitVSIDSScore(std::vector<int>);

void UpdateVSIDSCore(int var);

#endif
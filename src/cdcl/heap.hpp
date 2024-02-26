#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

#ifndef K_Heap_hpp
#define K_Heap_hpp

// A MinHeap for VSIDS Branching Heuristic


class Heap {
private:
    int size;
    int capacity;
    std::vector<int> heap;                              // int vector for the order of elems
    std::vector<double> score;                          // double vector for the scores of elems. score[1] == score of the 1st elem
    int getLChild(int i) const { return i * 2 + 1; }
    int getRChild(int i) const { return (i + 1) * 2; }
    int getParent(int i) const { return (i - 1) / 2; }

    bool scoreOperator(int v1, int v2) const { 
        return score[v1] < score[v2]; 
    }

    
public:
    Heap(int capacity);

    void heapifyUp(int);
    void heapifyDown(int);
    void insertKey(int);
    int getHeapSize() const;
    void display() const;
    int deleteKey();
};

//Constructor
Heap::Heap(int capacity){ 
    size = 0;
    this -> capacity = capacity;
    heap.resize(capacity);
};

void Heap::insertKey(int k) {
    
    //Increase amount of elems.
    size++;

    //Insert new key at the end
    int i = size - 1;
    heap[i] = k;
    
    //Fix the tree property
    //Move the element up until i => parent or root
    while (i != 0 && score[getParent(i) > score[i]]) {
        std::swap(heap[i], heap[getParent(i)]);
        i = getParent(i);
    }
}   




void Heap::heapifyUp(int c) {

  int tmp = heap[c];
  int p = getParent(c);

  while (c != 0 && scoreOperator(tmp, heap[p])) {
    heap[c] = heap[p];
  }
}

// Unfinished
void InitVSIDSScore(std::vector<int>);

void UpdateVSIDSCore(int var);

#endif
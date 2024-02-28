#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include "../../include/cdcl.hpp"

#ifndef K_Heap_hpp
#define K_Heap_hpp

// A MinHeap for VSIDS Branching Heuristic


class Heap {
private:
    int size;
    int capacity;
    std::vector<int> heap;                              // int vector for the order of elems (0-indexed)
    std::vector<double> score;                          // double vector for the scores of elems. score[1] == score of the 1st elem
    
    int getLChild(int i) const { return i * 2 + 1; }
    int getRChild(int i) const { return (i + 1) * 2; }
    int getParent(int i) const { return (i - 1) / 2; }

    bool scoreOperator(int v1, int v2) const { 
        return score[v1] < score[v2]; 
    }

    
public:
    Heap(int capacity);

    void insertKey(int);
    int getHeapSize() const;
    void display() const;
    int deleteKey();
    void initHeap(Heap);
    void heapify(int);
    int extractMin();
};

//Constructor
Heap::Heap(int capacity): size(0), capacity(capacity){ 
    heap.resize(capacity);
    score.resize(capacity);
};

void Heap::insertKey(int k) {

    std::vector<double> occs (1);

    if (size == capacity){
        std::cerr << "Heap Overflow\n";
        return;
    }
    
    //Increase amount of elems.
    size++;
    

    //Insert new key at the end
    int i = size - 1;
    heap[i] = k;

    printf("inserting element %i \n", k);

    occs[0] = vars[k].pos_occ + vars[k].neg_occ;
        

    score[k] = occs[0]; 
    printf("score for %i is %f \n", heap[i], score[k]);

    
    //score[k] = TODO : number of occs of k in cnf
    
    //Fix the tree property
    //Move the element up until i => parent or root
    while (i != 0 && score[heap[getParent(i)]] < score[heap[i]]) {
        printf("get parent results %i\n", heap[getParent(i)]);
        printf("elem %i has score %f and its parents score is %f \n", heap[i], score[heap[i]], score[getParent(i)]);
        if(score[getParent(i)] < score[i]) printf("they should change positions\n");
        std::swap(heap[i], heap[getParent(i)]);
        i = getParent(i);
    }
}   

int Heap::deleteKey() {
    if (size <= 0) {
        std::cerr << "Heap underflow\n";
        return -1; // Assuming -1 as an invalid value
    }

    int root = heap[0];
    heap[0] = heap[size - 1];
    score[root] = 0; // Reset the score of the removed variable
    size--;

    heapify(0);

    return root;
}   

void Heap::heapify(int i) {
    int l = getLChild(i);
    int r = getRChild(i);
    int smallest = i;

    //find smallest elem
    if (( l < size) && (score[l] < score[smallest])){
        smallest = l;
    }

    if ((r < size) && (score[r] < score[smallest])){
        smallest = r;
    }

    if (smallest != i){
        std::swap(heap[i], heap[smallest]);
        heapify(smallest);
    }
}

int Heap::extractMin( ) {
    if (size == 0){
        std::cerr << "Empty Heap\n";
        return -1;
    } else if (size == 1) {
        size--;
        return heap[0];
    }else {
        int root = heap[0];
        
        heap[0] = heap[size - 1];
        size--;
        heapify(0);
        
        return root;
    }
}

int Heap::getHeapSize() const {
    return size;
}

void Heap::display() const {

    int power = 0;
    int value = 1;
    for (int i = 0; i < size; ++i) {
        if(i == value){
            printf("\n");
            power += 1;
            value += (1 << power);
        }
        printf("(%i, %f) ", heap[i], score[heap[i]]);
        
    }
    printf("\n");
}

void Heap::initHeap(Heap heap){

    for (int i = 1; i < vars.size(); i++) {
        
        heap.insertKey(i);
        heap.display();

        printf("\n");
}
}

// Unfinished

#endif
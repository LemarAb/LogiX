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
    void scoreInit(std::vector<int>);
    void heapify(int);
    int extractMin();
};

//Constructor
Heap::Heap(int capacity): size(0), capacity(capacity){ 
    heap.resize(capacity);
    score.resize(capacity);
};

void Heap::insertKey(int k) {

    if (size == capacity){
        std::cerr << "Heap Overflow\n";
        return;
    }
    
    //Increase amount of elems.
    size++;

    //Insert new key at the end
    int i = size - 1;
    heap[i] = k;

    
    //score[k] = TODO : number of occs of k in cnf
    
    //Fix the tree property
    //Move the element up until i => parent or root
    while (i != 0 && heap[getParent(i)] > heap[i]) {
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

    heapifyDown(0);

    return root;
}

void Heap::heapify(int i) {
    int l = getLChild(i);
    int r = getRChild(i);
    int smallest = i;

    //find smallest elem
    if (( l < size) && (heap[l] < heap[smallest])){
        smallest = l;
    }

    if ((r < size) && (heap[r] < heap[smallest])){
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

void Heap::heapifyDown(int i) {
    int maxIdx = i;
    int leftChild = getLChild(i);
    int rightChild = getRChild(i);

    if (leftChild < size && score[heap[leftChild]] > score[heap[maxIdx]])
        maxIdx = leftChild;

    if (rightChild < size && score[heap[rightChild]] > score[heap[maxIdx]])
        maxIdx = rightChild;

    if (maxIdx != i) {
        std::swap(heap[i], heap[maxIdx]);
        heapifyDown(maxIdx);
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
            std::cout << "\n";
            power += 1;
            value += (1 << value);
        }
        std::cout << "(" << heap[i] << ") ";
    }
    std::cout << "\n";
}

void Heap::scoreInit(std::vector<int> formula){

    for (int i = 1; i < formula.size();  i++) {
        int var = formula[i];
        int count = 0;
        for (int j = 1; j < formula.size(); j++) {
            if (formula[j] == formula[i]) count++;
        }
        score[i-1] = count;
    }
}

// Unfinished

#endif
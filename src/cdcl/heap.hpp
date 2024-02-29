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

// A MaxHeap for VSIDS Branching Heuristic

class Heap {
private:
    
    int capacity;
    std::vector<int> heap;                              // int vector for the order of elems (0-indexed)
    std::vector<double> activity;                          // double vector for the scores of elems. score[1] == score of the 1st elem
    
    int getLChild(int i)  { return i * 2 + 1; }
    int getRChild(int i)  { return (i + 1) * 2; }
    int getParent(int i)  { return (i - 1) / 2; }

    
public:
    int size;
    Heap(int capacity);
    void insertKey(int);
    int getHeapSize() ;
    void display() ;
    int deleteKey();
    void initHeap();
    void heapify(int);
    int popMax();
};

//Constructor
Heap::Heap(int capacity): capacity(capacity){ 
    size = 0;
    heap.resize(capacity);
    activity.resize(capacity);
};

int Heap::getHeapSize()  {
    return size;
};

void Heap::insertKey(int k) {

    if (size == capacity){
        std::cerr << "Heap Overflow\n";
        return;
    }
    
    //Increase amount of elems.
    size++;
    printf("size == %i,\n", getHeapSize());
    

    //Insert new key at the end
    int i = size - 1;
    heap[i] = k;

    printf("inserting element %i \n", k);

    activity[k] = vars[k].pos_occ + vars[k].neg_occ;
    printf("score for %i is %f \n", heap[i], activity[k]);

    
    //score[k] = TODO : number of occs of k in cnf
    
    //Fix the tree property
    //Move the element up until i => parent or root
    while (i != 0 && activity[heap[getParent(i)]] < activity[heap[i]]) {
        printf("get parent results %i\n", heap[getParent(i)]);
        printf("elem %i has score %f and its parents score is %f \n", heap[i], activity[heap[i]], activity[getParent(i)]);
        std::swap(heap[i], heap[getParent(i)]);
        i = getParent(i);
    }
};   

int Heap::deleteKey() {
    if (size <= 0) {
        std::cerr << "Heap underflow\n";
        return -1; // Assuming -1 as an invalid value
    }

    int root = heap[0];
    heap[0] = heap[size - 1];
    activity[root] = 0; // Reset the score of the removed variable
    size--;

    heapify(0);

    return root;
};  

void Heap::heapify(int i) {
    int l = getLChild(i);
    int r = getRChild(i);
    int largest = i;

    //find biggest elem

    //check if left child is bigger
    if (( l < size) && (activity[heap[l]] > activity[heap[largest]])){
        largest = l;
    }

    if ((r < size) && (activity[heap[r]] > activity[heap[largest]])){
        largest = r;
    }

    if (largest != i){
        std::swap(heap[i], heap[largest]);
        heapify(largest);
    }
};

int Heap::popMax( ) {
    printf("%d size\n", getHeapSize());
    if (getHeapSize() == 0){
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
};

void Heap::display()  {

    int power = 0;
    int value = 1;
    for (int i = 0; i < size; ++i) {
        if(i == value){
            printf("\n");
            power += 1;
            value += (1 << power);
        }
        printf("(%i, %f) ", heap[i], activity[heap[i]]);
        
    }
    printf("\n");
};

void Heap::initHeap(){

    for (int i = 1; i < vars.size(); i++) {
        
        this->insertKey(i);
        this->display();

        printf("\n");
};
}


#endif
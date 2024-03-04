#include <climits>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <map>
#include "../../include/cdcl.hpp"
#include "intmap.hpp"
#include "vector.hpp"
#include <cassert>

#ifndef K_Heap_hpp
#define K_Heap_hpp

// A MaxHeap for VSIDS Branching Heuristic

IntMap<int, double> act;

struct VariableOrderActLT {
    const IntMap<int, double>& activity;
    bool operator () (int x, int y) const { return activity[x] > activity[y]; }
    VariableOrderActLT(const IntMap<int, double>& act) : activity(act) { }
};  

VariableOrderActLT lt(act);

template<class K, class Comp, class MkIndex = MkIndexDefault<K> >
class Heap {
    vec<K> heap;               //currently as int but can be Variable, probably should be too.
    IntMap<K,int,MkIndex>  indices;      //double for the scores of the variables
    Comp  lt;              //Comparator for the heap
    
    static inline int getLChild(int i)  { return i * 2 + 1; }
    static inline int getRChild(int i)  { return (i + 1) * 2; }
    static inline int getParent(int i)  { return (i - 1) >> 1; }

    void heapUp(int i){
        K x = heap[i];
        int p = getParent(i);
        
        while (i != 0 && lt(x, heap[p])){
            heap[i] = heap[p];
            indices[heap[p]] = i;
            i = p;
            p = getParent(p);
        }
        heap[i] = x;
        indices[x] = i;
    }

    void heapDown(int i) {
        K x = heap[i];
        while (getLChild(i) < heap.size()){
            int child = getRChild(i) < heap.size() && lt(heap[getRChild(i)], heap[getLChild(i)]) ? getRChild(i) : getLChild(i);
            if (!lt(heap[child], x)) break;
            heap[i] = heap[child];
            indices[heap[i]] = i;
            i = child;
        }
        heap[i] = x;
        indices[x] = i;
    }
    
public:
    Heap(const Comp& c, MkIndex _index = MkIndex()): indices(_index), lt(c) { }
    
    int getHeapSize() const { return heap.size(); } 
    bool empty() const { return heap.size() == 0; }
    bool inHeap(int var) const { return indices.has(var) && indices[var] >= 0; }

    int operator[](int ind) const {assert(ind < heap.size()); return heap[ind]; } 

    void decrease (int i) { assert(inHeap(i)); heapUp(indices[i]); } 
    void increase (int i) { assert(inHeap(i)); heapDown(indices[i]); }

    void insert (K i) {
        indices.reserve(i, -1);
        assert(!inHeap(i));

        indices[i] = heap.size();
        heap.push(i);
        heapUp(indices[i]);
    }

    void update (K i) {
        if(!inHeap(i))  insert(i);
        else {
            heapUp(indices[i]);
            heapDown(indices[i]);
        }
    }

    void remove (K i) {
        assert(inHeap(i));

        int i_pos = indices[i];
        indices[i] = -1;

        if (i_pos < heap.size() - 1) {
            heap[i_pos] = heap.last();
            indices[heap[i_pos]] = i_pos;
            heap.pop();
            heapDown(i_pos);
        } else {
            heap.pop();
        }
    }

    K removeMin() {
        int i = heap[0];
        heap[0] = heap.last();
        indices[heap[0]] = 0;
        indices[i] = -1;
        heap.pop();
        if (heap.size() > 1) heapDown(0);
        return i;
    }

    void createHeap(const vec<int>& newVars) {
        for (int i = 0; i < heap.size(); i++) 
            indices[heap[i]] = -1;
        heap.clear();

        for(int i = 0; i < newVars.size(); i++) {
            assert(indices.has[newVars[i]]);
            indices[newVars[i]] = i;
            heap.push(newVars[i]);
        }

        for (int i = heap.size() / 2; i >= 0; i--) 
            heapDown(i);
    }

    void clear(bool dispose = false) {
        for (int i = 0; i < heap.size(); i++) 
            indices[heap[i]] = -1;
        heap.clear(dispose);
    }

    double var_inc = 5.0;
    
    void varIncActivity(int var) {
        act[var] += var_inc;
        if (vsidsheap.inHeap(var)) vsidsheap.decrease(var); 
    };

    void display() const {
        displayHelper(0, "");
    }; 

    private:
    void displayHelper(int i, const std::string& prefix) const {
        if (i < heap.size()) {
            if (i != 0) {
                std::cout << prefix << "|--";
            }

            std::cout << "(" << heap[i] << ")\n'";

            int left = getLChild(i);
            int right = getRChild(i);

            displayHelper(left, prefix + (right < heap.size() ? "|   " : "    "));
            displayHelper(right, prefix + "    ");
        }
    }
}; 

Heap<int, VariableOrderActLT> vsidsheap(lt); 



#endif



/*int power = 0;
        int value = 1;
        for (int i = 0; i < heap.size; ++i) {
            if(i == value){
                printf("\n");
                power += 1;
                value += (1 << power);
        }
        printf("(%i, %f) ", i, activity[i].second);
    }
    printf("\n");
    */
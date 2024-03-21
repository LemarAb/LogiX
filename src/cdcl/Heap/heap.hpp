#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

#ifndef K_Heap_hpp
#define K_Heap_hpp

// A MaxHeap for VSIDS Branching Heuristic
// The heap is implemented as a binary tree, with the root of the tree being the
// variable with the highest activity.
void afterExtractOrderAct(int i);

struct VariableOrderActLT {
  const std::vector<double> &activity;
  bool operator()(int x, int y) const { return activity[x] > activity[y]; }
  VariableOrderActLT(const std::vector<double> &act) : activity(act) {}
};

template <class K, class Comp> class Heap {
  std::vector<K> heap;      // true heap, first element is always the biggest in
                            // score, saves them in their order
  std::vector<int> indices; // Locations of the vars in Heap
  Comp lt;                  // Comparator for the heap

  static inline int getLChild(int i) { return i * 2 + 1; }
  static inline int getRChild(int i) { return (i + 1) * 2; }
  static inline int getParent(int i) { return (i - 1) >> 1; }

  void siftUp(int i) {
    K x = heap[i];
    int p = getParent(i);

    while (i != 0 && lt(x, heap[p])) {
      heap[i] = heap[p];
      indices[heap[p]] = i;
      i = p;
      p = getParent(p);
    }
    heap[i] = x;
    indices[x] = i;
  }

  void siftDown(int i) {
    K x = heap[i];
    while (getLChild(i) < heap.size()) {
      int child = getRChild(i) < heap.size() &&
                          lt(heap[getRChild(i)], heap[getLChild(i)])
                      ? getRChild(i)
                      : getLChild(i);
      if (!lt(heap[child], x))
        break;
      heap[i] = heap[child];
      indices[heap[i]] = i;
      i = child;
    }
    heap[i] = x;
    indices[x] = i;
  }

public:
  Heap(const Comp &c) : lt(c) {}

  int peek() const { return heap[0]; }
  int getHeapSize() const { return heap.size(); }
  bool empty() const { return heap.size() == 0; }
  bool inHeap(int var) const {
    return var < indices.size() && indices.at(var) >= 0;
  }

  int operator[](int ind) const {
    assert(ind < heap.size());
    return heap[ind];
  }

  void decrease(int i) {
    assert(inHeap(i));
    siftUp(indices[i]);
  }

  void increase(int i) {
    assert(inHeap(i));
    siftDown(indices[i]);
  }

  void insert(K i) {
    if (i >= indices.size()) {
      indices.resize(i + 1, -1);
    }
    indices[i] = -1;
    assert(!inHeap(i));

    indices[i] = heap.size();
    heap.push_back(i);
    siftUp(indices[i]);
  }

  void update(K i) {
    if (!inHeap(i))
      insert(i);

    siftUp(indices[i]);
    siftDown(indices[i]);
  }

  void remove(K i) {
    assert(inHeap(i));

    int i_pos = indices[i];
    indices[i] = -1;

    if (i_pos < heap.size() - 1) {
      heap[i_pos] = heap.back();
      indices[heap[i_pos]] = i_pos;
      heap.pop_back();
      siftDown(i_pos);
    } else {
      heap.pop_back();
    }
  }

  K removeMax() {
    int i = heap[0];
    heap[0] = heap.back();

      indices[heap[0]] = 0;

    
      indices[i] = -1;

    heap.pop_back();

    if (heap.size() > 1) {
      siftDown(0);
    }

    // afterExtractOrderAct(i);
    return i;
  }

  void createHeap() {
    indices.resize(numOfVars, -1);

    for (int i = 0; i < numOfVars ; i++) {
      insert(i+1);
    }

    for (int i = heap.size() / 2; i >= 0; i--)
      siftDown(i);
  }

  void rebuild() {
    // Clear the heap and indices
    heap.clear();
    indices.clear();

    // Add variables from 1 to numOfVars to the heap and indices
    for (int i = 1; i <= numOfVars; ++i) {
      heap.push_back(i);
      indices[i] = i - 1;
    }

    // Restore the heap property
    for (int i = heap.size() / 2; i >= 0; --i) {
      siftDown(i);
    }
  }

  void clear(bool dispose = false) {
    for (int i = 0; i < heap.size(); i++)
      indices[heap[i]] = -1;
    heap.clear(dispose);
  }

  void displayIndices() const {
    for (int i = 0; i < indices.size(); ++i) {
      std::cout << "Key: " << i << ", Value: " << indices[i] << '\n';
    }
  }

  void displaySize() const {
    std::cout << "Size of heap: " << heap.size() << '\n';
    std::cout << "Size of indices: " << indices.size() << '\n';
  }

  void display(const std::vector<double> &act) const {
    displayHelper(0, "", act);
  };

private:
  void displayHelper(int i, const std::string &prefix,
                     const std::vector<double> &act) const {
    if (i < heap.size()) {
      if (i != 0) {
        std::cout << prefix << "|--";
      }

      std::cout << "(" << heap[i] << ", " << act.at(heap[i]) << ", "
                << vars[heap[i]].getValue() << ")\n'";

      int left = getLChild(i);
      int right = getRChild(i);

      displayHelper(left, prefix + (right < heap.size() ? "|   " : "    "),
                    act);
      displayHelper(right, prefix + "    ", act);
    }
  }
};
#endif

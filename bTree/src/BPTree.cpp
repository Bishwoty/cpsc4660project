#include <BPTree.h>
#include <algorithm>
#include <iostream>
using namespace std;

// NODE ************************************************************************
BPTree::Node::Node(int treeOrder, bool isLeaf)
    : order(treeOrder), leaf(isLeaf) {
  children.resize(treeOrder, nullptr);
  parent = nullptr;
}

BPTree::Node::~Node() {}
//******************************************************************************

BPTree::BPTree(int treeOrder) : order(treeOrder), root(nullptr) {}

BPTree::~BPTree() {
  if (root) {
    delete root;
    root = nullptr;
  }
}

void BPTree::insert(int num) {
  if (!root) {
    root = new Node(order, true);
  }
  root = this->insert(num, root);
}

BPTree::Node* BPTree::insert(int k, Node* T) {
  // Following algorithim in paper
  // 1. N <- T
  Node* N = T;
  // 2. While N is a non-leaf node
  while (!N->leaf) {
    if (k > N->values.back()) {
      N = N->children.back();
    } else {
      for (int i = 0; i < N->values.size(); i++) {
        if (k <= N->values.at(i)) {
          N = N->children.at(i);
        }
      }
    }
  }
  // 3. Search N for key, if found, return "record already exists" and exit
  if (std::find(N->values.begin(), N->values.end(), k) != N->values.end()) {
    return T;
  }
  // 4. If N is under full then insert k into N with proper order
  if (N->values.size() < N->order - 1) {
    N->values.push_back(k);
    std::sort(N->values.begin(), N->values.end());
  }
  // 5. If N is full and at least one of N's right siblings is under full then
  // Rshift(N, k)
  else if (!leftSibFull(N)) {
    rightShift(N, k);
  }
  // 6. If N is full and at least one of N's left siblings is under full then
  // Lshift(N, k)
  else if (!rightSibFull(N)) {
    leftShift(N, k);
  }
  // 7. If N and all N's siblings are full, then Split(N, k)
  else {
    split(N, k);
  }
  // 8 .return the root
  while (T->parent) {
    T = T->parent;
  }
  return T;
}

bool BPTree::leftSibFull(Node* N) {  // finish
  if (!N->parent) {
    return true Node * P = N->parent;
  }
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == N) {
      break;
    }
  }
}

bool BPTree::leftSibFull(Node* N) {  // finish
  if (!N->parent) {
    return true Node * P = N->parent;
  }
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == N) {
      break;
    }
  }
}

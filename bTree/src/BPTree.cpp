#include <BPTree.h>
#include <algorithm>  //std::sort
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
  root = this->insert(root, num);
}

BPTree::Node* BPTree::insert(Node* T, int k) {
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
          break;
        }
      }
    }
  }
  // 3. Search N for key, if found, return "record already exists" and exit
  if (std::find(N->values.begin(), N->values.end(), k) != N->values.end()) {
    return T;
  }

  Node* sibR = SibRoom(N, false);
  Node* sibL = SibRoom(N, true);

  // 4. If N is under full then insert k into N with proper order
  if (N->values.size() < N->order - 1) {
    N->values.push_back(k);
    std::sort(N->values.begin(), N->values.end());
  }
  // 5. If N is full and at least one of N's right siblings is under full then
  // Rshift(N, k)
  else if (sibR) {
    rightShift(N, sibR, k);
  }
  // 6. If N is full and at least one of N's left siblings is under full then
  // Lshift(N, k)
  else if (sibL) {
    leftShift(N, sibL, k);
  }
  // 7. If N and all N's siblings are full, then Split(N, k)
  else {
    // split(N, k);
  }
  // 8 .return the root
  while (T->parent) {
    T = T->parent;
  }
  return T;
}

BPTree::Node* BPTree::SibRoom(Node* N, bool left) {
  if (!N->parent) {
    return nullptr;
  }
  Node* P = N->parent;
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == N) {
      break;
    }
  }
  for (int j = (left) ? i - 1 : i + 1; (left) ? j >= 0 : j < P->children.size();
       (left) ? j-- : j++) {
    if (P->children.at(j)) {
      if (P->children.at(j)->values.size() < P->children.at(j)->order - 1) {
        return P->children.at(i - 1);
      }
    }
  }
  return nullptr;
}

void BPTree::rightShift(Node* N, Node* S, int k) {
  N->values.push_back(k);
  std::sort(N->values.begin(), N->values.end());
  int temp = N->values.back();
  N->values.erase(N->values.end() - 1);

  // replace key in parent
  Node* P = N->parent;
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == N) {
      break;
    }
  }
  P->values.at(i) = N->values.back();

  insert(S, temp);
}

void BPTree::leftShift(Node* N, Node* S, int k) {
  N->values.push_back(k);
  std::sort(N->values.begin(), N->values.end());
  int temp = N->values.front();
  N->values.erase(N->values.begin());

  // replace key in parent
  Node* P = S->parent;
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == S) {
      break;
    }
  }
  P->values.at(i) = temp;

  insert(S, temp);
}

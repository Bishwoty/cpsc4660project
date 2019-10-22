#include <BPTree.h>
#include <algorithm>  //std::sort
#include <iostream>
using namespace std;

// NODE ************************************************************************
BPTree::Node::Node(int treeOrder, bool isLeaf)
    : order(treeOrder), leaf(isLeaf) {
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

void BPTree::split(Node* N, int k) {
  // If N is a leaf
  if (N->leaf) {
    // Accommodate a new leaf NN
    Node* NN = new Node(N->order, true);

    // kp <- the last key in N
    int kp = N->values.back();

    // Collect k and all keys in N into sequence S and sort S increasingly
    vector<int> S;
    S.push_back(k);
    while (N->values.size() > 0) {
      S.push_back(N->values.back());
      N->values.pop_back();
    }

    // Equally divide S into two halves and install the first and second half
    // into nodes N and NN respectivly
    for (int i = 0; i < S.size(); i++) {
      if (i < S.size() / 2) {
        N->values.push_back(S.at(i));
      } else {
        NN->values.push_back(S.at(i));
      }
    }

    // k1 <- the last key in N
    // k2 <- the last key in NN
    int k1 = N->values.back();
    int k2 = NN->values.back();

    // Replace the key equal to kp in parent(N) with k1
    *std::find(N->parent->values.begin(), N->parent->values.end(), kp) = k1;

    // If vol(parent(N)) < n
    if (N->parent->values.size() < N->parent->order - 1) {
      // Insert k2 to patent(N) and create a pointer refering to NN
      N->parent->values.push_back(k2);
      N->parent->children.push_back(NN);
    }

    // Else /* vol(parent(N)) = n */
    else {
      split(N->parent, k2);
    }

  }

  // Else /* N is a full internal node involving split */
  else {
  }
}

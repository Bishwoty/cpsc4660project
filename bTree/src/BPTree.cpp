#include <BPTree.h>
#include <algorithm>  //std::sort
#include <iostream>
#include <utility>  //std::pair
using namespace std;

// NODE ************************************************************************
BPTree::Node::Node(bool isLeaf) : leaf(isLeaf), parent(nullptr) {}

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
    root = new Node(true);
  }
  root = this->insert(root, num);
}

BPTree::Node* BPTree::insert(Node* T, int k) {
  cout << "insert\n";
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
  if (N->values.size() < order - 1) {
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
    split(N, k);
  }
  // 8 .return the root
  while (T->parent) {
    T = T->parent;
  }
  return T;
}

BPTree::Node* BPTree::SibRoom(Node* N, bool left) {
  cout << "SibRoom\n";
  if (!N->parent) {
    return nullptr;
  }

  Node* P = N->parent;
  int i;
  for (i = 0; i < P->children.size(); i++) {
    if (P->children.at(i) == N) {
      cout << i << "\n";
      break;
    }
  }
  // cout << P->children.at(0)->values.at(0) << P->children.at(1)->values.at(0);
  for (int j = (left) ? i - 1 : i + 1; (left) ? j >= 0 : j < P->children.size();
       (left) ? j-- : j++) {
    if (P->children.at(j)) {
      if (P->children.at(j)->values.size() < order - 1) {
        return P->children.at(i - 1);
      }
    }
  }
  return nullptr;
}

void BPTree::rightShift(Node* N, Node* S, int k) {
  cout << "rShift\n";
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
  cout << "lShift\n";
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
  cout << "splitLeaf\n";
  // N is a leaf

  // Create new root if needed
  bool newRoot = false;
  if (!N->parent) {
    N->parent = new Node(false);
    N->parent->children.push_back(N);
    N->parent->values.push_back(N->values.back());
    newRoot = true;
  }

  // Accommodate a new leaf NN
  Node* NN = new Node(true);

  // kp <- the last key in N
  int kp = N->values.back();

  // Collect k and all keys in N into sequence S and sort S increasingly
  vector<int> S;
  S.push_back(k);
  while (N->values.size() > 0) {
    S.push_back(N->values.back());
    N->values.pop_back();
  }
  std::sort(S.begin(), S.end());

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
  if (N->parent->values.size() < order - 1) {
    // Insert k2 to parent(N) and create a pointer refering to NN
    N->parent->children.push_back(NN);
    NN->parent = N->parent;
    if (!newRoot) {
      N->parent->values.push_back(k2);
    }

    // Make sure new node is in correct position
    for (int i = NN->values.size() - 1; i > 0; i--) {
      if (NN->values.at(i) < NN->values.at(i - 1)) {
        std::swap(NN->values.at(i), NN->values.at(i - 1));
        std::swap(NN->children.at(i + 1), NN->children.at(i));
      }
    }
  }

  // Else /* vol(parent(N)) = n */
  else {
    split(N->parent, k2, NN);
  }
}

void BPTree::split(Node* N, int k, Node* C) {
  cout << "split\n";
  // N is a full internal node involving split

  // Create new root if needed
  bool newRoot = false;
  if (!N->parent) {
    N->parent = new Node(false);
    N->parent->children.push_back(N);
    newRoot = true;
  }

  // Accommodate a new node NN
  Node* NN = new Node(false);
  cout << "oops" << endl;
  // Collect k and all keys in N into sequence S and sort S increasingly
  auto it = N->values.begin();
  auto it2 = N->children.begin();
  ++it2;
  for (; it != N->values.end(); ++it) {
    if (k < *it) {
      break;
    }
    ++it2;
  }
  N->values.insert(it, k);
  N->children.insert(it2, C);

  // Pick the median element in S as km and install the elements smaller than
  // km into N and install the elements larger than km into NN
  int km = N->values.at(N->values.size() / 2);
  NN->children.push_back(N->children.back());
  N->children.pop_back();

  for (int i = 0; i < N->values.size(); i++) {
    if (N->values.at(i) > km) {
      NN->values.push_back(N->values.at(i));
      N->values.erase(N->values.begin() + i);
      NN->children.push_back(N->children.at(i));
      N->children.erase(N->children.begin() + i);
      i--;
    }
  }
  // If vol(parent(N)) < n
  if (N->parent->values.size() < order - 1) {
    // Insert km to parent(N) and create a pointer refering to NN
    if (!newRoot) {
      N->parent->values.push_back(km);
    }
    N->parent->children.push_back(NN);
    NN->parent = N->parent;
    // Make sure new node is in correct position
    for (int i = NN->values.size() - 1; i > 0; i--) {
      if (NN->values.at(i) < NN->values.at(i - 1)) {
        std::swap(NN->values.at(i), NN->values.at(i - 1));
        std::swap(NN->children.at(i + 1), NN->children.at(i));
      }
    }
  }

  // Else /* vol(parent(N)) = n */
  else {
    split(N->parent, km, NN);
  }
}

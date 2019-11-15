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
    deleteTree(root);
    root = nullptr;
  }
}

bool BPTree::insert(int key) {
  if (!root) {
    root = new Node(true);
  }

  // cout << "inserting " << key << endl;
  // Following algorithim in paper
  // 1. N <- T
  Node* N = root;
  // 2. While N is a non-leaf node
  while (!N->leaf) {
    if (key > N->values.back()) {
      N = N->children.back();
    } else {
      for (int i = 0; i < N->values.size(); i++) {
        if (key <= N->values.at(i)) {
          N = N->children.at(i);
          break;
        }
      }
    }
  }

  // 3. Search N for key, if found, return "record already exists" and exit
  for (int i = 0; i < N->values.size(); i++) {
    if (N->values.at(i) == key) {
      return false;
    }
  }

  Node* sibR = Sibling(N, false);
  Node* sibL = Sibling(N, true);

  // 4. If N is under full then insert key into N with proper order
  if (N->values.size() < order - 1) {
    auto pos = N->values.begin();
    for (; pos != N->values.end(); ++pos) {
      if (*pos > key) {
        break;
      }
    }
    N->values.insert(pos, key);
  }
  // 5. If N is full and N's right sibling is under full then
  // Rshift(N, key)
  else if (sibR && sibR->values.size() < order - 1) {
    rightShift(N, sibR, key);
  }
  // 6. If N is full and N's left sibling is under full then
  // Lshift(N, key)
  else if (sibL && sibL->values.size() < order - 1) {
    leftShift(N, sibL, key);
  }
  // 7. If N and all N's siblings are full, then Split(N, key)
  else {
    split(N, key);
  }
  // 8 .return the root
  while (root->parent) {
    root = root->parent;
  }
  return true;
}

bool BPTree::find(int key) {
  // cout << "finding " << key << endl;
  Node* N = root;

  // If N is null, the tree is empty, return false
  if (!N) {
    return false;
  }

  // While N is a non-leaf node
  while (!N->leaf) {
    if (key > N->values.back()) {
      N = N->children.back();
    } else {
      for (int i = 0; i < N->values.size(); i++) {
        if (key <= N->values.at(i)) {
          N = N->children.at(i);
          break;
        }
      }
    }
  }

  // Search N for key
  for (int i = 0; i < N->values.size(); i++) {
    if (N->values.at(i) == key) {
      return true;
    }
  }
  return false;
}

bool BPTree::remove(int key) {
  // cout << "removing " << key << endl;
  Node* N = root;
  // While N is a non-leaf node
  while (!N->leaf) {
    if (key > N->values.back()) {
      N = N->children.back();
    } else {
      for (int i = 0; i < N->values.size(); i++) {
        if (key <= N->values.at(i)) {
          N = N->children.at(i);
          break;
        }
      }
    }
  }

  auto position = std::find(N->values.begin(), N->values.end(), key);
  // Search N for key
  if (position == N->values.end()) {
    // Key doesnt exist, do nothing and return
    return false;
  }

  // Delete key
  N->values.erase(position);

  // Ceiling of order / 2, needed to check underflow
  int minValues = (order % 2) ? order / 2 + 1 : order / 2;

  // If root
  if (N == root) {
    // If last elemet is deleted, delete root node (tree is empty)
    if (N->values.size() < 1) {
      delete N;
      root = nullptr;
    }
  }
  // If underflow not in root
  else if (N->values.size() < minValues) {
    // Find siblings
    Node* sibR = Sibling(N, false);
    Node* sibL = Sibling(N, true);

    // Try to redistibute with left sibling
    if (sibL && sibL->values.size() > minValues) {
      // cout << "Leaf redistribute left" << endl;
      N->values.insert(N->values.begin(), sibL->values.back());
      sibL->values.pop_back();
      orginize(N->parent);
    }
    // Try to redistibute with right sibling
    else if (sibR && sibR->values.size() > minValues) {
      // cout << "Leaf redistribute right" << endl;
      N->values.push_back(sibR->values.front());
      sibR->values.erase(sibR->values.begin());
      orginize(N->parent);
    }
    // Merge with a sibling
    else {
      // cout << "Leaf merge" << endl;
      Node* merge = (sibL ? sibL : sibR);
      while (N->values.size() > 0) {
        merge->values.push_back(N->values.back());
        N->values.pop_back();
      }

      // Sort merged node
      std::sort(merge->values.begin(), merge->values.end());

      // Find and delete empty node
      Node* P = N->parent;
      int pos = 0;
      for (; pos < P->children.size(); pos++) {
        if (P->children.at(pos) == N) {
          break;
        }
      }
      if (pos >= P->children.size()) {
        // cout << "ERROR: My parent doesn't know me!" << endl;
      }

      delete P->children.at(pos);
      P->children.erase(P->children.begin() + pos);
      N = nullptr;

      // Update parent
      orginize(merge->parent);

      // If parent underflows
      if (P->children.size() < minValues) {
        concatinate(P);
      }
    }
  }
  return true;
}

void BPTree::deleteTree(Node* N) {
  for (int i = 0; i < N->children.size(); i++) {
    deleteTree(N->children.at(i));
  }
  delete N;
}

BPTree::Node* BPTree::Sibling(Node* N, bool left) {
  // cout << "Sibling " << (left ? "left" : "right") << endl;
  if (!N->parent) {
    return nullptr;
  }
  Node* P = N->parent;
  int pos = 0;
  for (; pos < P->children.size(); pos++) {
    if (P->children.at(pos) == N) {
      break;
    }
  }

  if (pos >= P->children.size()) {
    // cout << "ERROR: My parent doesn't know me!" << endl;
  }
  if (left) {
    if (pos - 1 >= 0) {
      return P->children.at(pos - 1);
    }
  } else {
    if (pos + 1 < P->children.size()) {
      return P->children.at(pos + 1);
    }
  }
  return nullptr;
}

void BPTree::rightShift(Node* N, Node* S, int k) {
  // cout << "rShift\n";
  N->values.push_back(k);
  std::sort(N->values.begin(), N->values.end());
  int temp = N->values.back();
  N->values.pop_back();

  S->values.insert(S->values.begin(), temp);

  orginize(N->parent);
}

void BPTree::leftShift(Node* N, Node* S, int k) {
  // cout << "lShift\n";
  N->values.push_back(k);
  std::sort(N->values.begin(), N->values.end());
  int temp = N->values.front();
  N->values.erase(N->values.begin());

  S->values.push_back(temp);

  orginize(N->parent);
}

void BPTree::split(Node* N, int k) {
  // cout << "splitLeaf\n";
  // N is a leaf

  // Create new root if needed
  if (!N->parent) {
    N->parent = new Node(false);
    N->parent->children.push_back(N);
  }

  // Accommodate a new leaf NN
  Node* NN = new Node(true);

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

  // If vol(parent(N)) < n
  if (N->parent->children.size() < order) {
    // Create a pointer in parent(N) refering to NN
    N->parent->children.push_back(NN);
    NN->parent = N->parent;

    // Make sure new node is in correct position in parent
    orginize(N->parent);
  }

  // Else /* vol(parent(N)) = n */
  else {
    split(N->parent, NN);
  }
}

void BPTree::split(Node* N, Node* C) {
  // cout << "split\n";
  // N is a full internal node involving split

  // Create new root if needed
  if (!N->parent) {
    N->parent = new Node(false);
    N->parent->children.push_back(N);
  }

  // Accommodate a new node NN
  Node* NN = new Node(false);

  // Create a refrence to C in N
  auto pos = N->children.begin();
  for (; pos != N->children.end(); ++pos) {
    if (C->values.back() < (*pos)->values.back()) {
      break;
    }
  }
  N->children.insert(pos, C);
  C->parent = N;

  // Pick the median element in N and remove the elements after it from N
  // and install them, into NN
  int middle = N->children.size() / 2;
  while (N->children.size() > middle) {
    NN->children.push_back(N->children.at(middle));
    N->children.at(middle)->parent = NN;
    N->children.erase(N->children.begin() + middle);
  }

  // Orginize N and NN
  orginize(N);
  orginize(NN);

  // If vol(parent(N)) < n
  if (N->parent->children.size() < order) {
    // Create a pointer in parent(N) refering to NN
    N->parent->children.push_back(NN);
    NN->parent = N->parent;

    // Make sure new node is in correct position in parent
    orginize(N->parent);
  }

  // Else /* vol(parent(N)) = n */
  else {
    split(N->parent, NN);
  }
}

void BPTree::orginize(Node* N) {
  N->values.clear();
  bool again = true;
  while (again) {
    again = false;
    for (int i = 0; i < N->children.size() - 1; i++) {
      if (N->children.at(i)->values.back() >
          N->children.at(i + 1)->values.back()) {
        std::swap(N->children.at(i), N->children.at(i + 1));
        again = true;
      }
    }
  }
  for (int i = 0; i < N->children.size() - 1; i++) {
    N->values.push_back(max(N->children.at(i)));
  }
}

void BPTree::concatinate(Node* N) {
  // cout << "concatinate" << endl;
  // If N is the root and has only one child
  if (N == root && N->children.size() == 1) {
    // Make child the root, delete old root
    root = N->children.front();
    root->parent = nullptr;
    delete N;
    return;
  }

  // Ceiling of order / 2, needed to check underflow
  int minChildren = (order % 2) ? order / 2 + 1 : order / 2;

  // Find siblings
  Node* sibR = Sibling(N, false);
  Node* sibL = Sibling(N, true);

  // Try to redistibute with left sibling
  if (sibL && sibL->children.size() > minChildren) {
    N->children.insert(N->children.begin(), sibL->children.back());
    sibL->children.back()->parent = N;
    sibL->children.pop_back();
    orginize(N);
    orginize(sibL);
    orginize(N->parent);
  }
  // Try to redistibute with right sibling
  else if (sibR && sibR->children.size() > minChildren) {
    N->children.push_back(sibR->children.front());
    sibR->children.front()->parent = N;
    sibR->children.erase(sibR->children.begin());
    orginize(N);
    orginize(sibR);
    orginize(N->parent);
  }

  // Merge with a sibling
  else {
    Node* merge = (sibL ? sibL : sibR);
    while (N->children.size() > 0) {
      merge->children.push_back(N->children.back());
      N->children.back()->parent = merge;
      N->children.pop_back();
    }

    // Sort merged node and update seperators
    orginize(merge);

    // Find and delete empty node
    Node* P = N->parent;
    int pos = 0;
    for (; pos < P->children.size(); pos++) {
      if (P->children.at(pos) == N) {
        break;
      }
    }
    if (pos >= P->children.size()) {
      // cout << "ERROR: My parent doesn't know me!" << endl;
    }

    delete P->children.at(pos);
    P->children.erase(P->children.begin() + pos);
    N = nullptr;

    // Update parent
    orginize(merge->parent);

    // If parent underflows
    if (P->children.size() < minChildren) {
      concatinate(P);
    }
  }
}

int BPTree::max(Node* N) {
  while (!N->leaf) {
    N = N->children.back();
  }
  return N->values.back();
}

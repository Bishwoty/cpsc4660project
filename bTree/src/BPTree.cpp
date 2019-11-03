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

bool BPTree::insert(int key) {
  if (!root) {
    root = new Node(true);
  }

  cout << "inserting " << key << endl;
  // Following algorithim in paper
  // 1. N <- T
  Node* N = root;
  // 2. While N is a non-leaf node
  while (!N->leaf) {
    if (key > N->values.back()) {
      cout << key << " > " << N->values.back() << endl;
      N = N->children.back();
    } else {
      for (int i = 0; i < N->values.size(); i++) {
        if (key <= N->values.at(i)) {
          cout << key << " <= " << N->values.at(i) << endl;
          N = N->children.at(i);
          break;
        }
      }
    }
  }

  if (key == 15) {
    cout << N << ' ' << N->parent->children.at(0) << endl;

    for (int i = 0; i < N->parent->children.at(1)->values.size(); i++) {
      cout << N->parent->children.at(1)->values.at(i) << ' ';
    }
    cout << endl;
  }

  // 3. Search N for key, if found, return "record already exists" and exit
  if (std::find(N->values.begin(), N->values.end(), key) != N->values.end()) {
    return false;
  }

  Node* sibR = SibRoom(N, false);
  Node* sibL = SibRoom(N, true);

  cout << sibL << ' ' << sibL << endl;

  // 4. If N is under full then insert key into N with proper order
  if (N->values.size() < order - 1) {
    N->values.push_back(key);
    std::sort(N->values.begin(), N->values.end());
  }
  // 5. If N is full and at least one of N's right siblings is under full then
  // Rshift(N, key)
  else if (sibR) {
    rightShift(N, sibR, key);
  }
  // 6. If N is full and at least one of N's left siblings is under full then
  // Lshift(N, key)
  else if (sibL) {
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
  cout << "finding " << key << endl;
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

  // Search N for key
  if (std::find(N->values.begin(), N->values.end(), key) == N->values.end()) {
    return false;
  } else {
    return true;
  }
}

BPTree::Node* BPTree::SibRoom(Node* N, bool left) {
  cout << "SibRoom " << (left ? "left" : "right") << endl;
  if (!N->parent) {
    return nullptr;
  }
  cout << "has parent" << endl;
  Node* P = N->parent;
  int pos = 0;
  for (; pos < P->children.size(); pos++) {
    if (P->children.at(pos) == N) {
      break;
    }
  }

  if (pos >= P->children.size()) {
    cout << "ERROR!" << endl;
  }
  if (left) {
    if (pos - 1 >= 0) {
      cout << "true" << endl;
      if (P->children.at(pos - 1)->values.size() < order - 1) {
        cout << "true" << endl;
        return P->children.at(pos - 1);
      }
    }
  } else {
    if (pos + 1 < P->children.size()) {
      if (P->children.at(pos + 1)->values.size() < order - 1) {
        return P->children.at(pos + 1);
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
  N->values.pop_back();

  S->values.push_back(temp);

  orginize(N->parent);
}

void BPTree::leftShift(Node* N, Node* S, int k) {
  cout << "lShift\n";
  N->values.push_back(k);
  std::sort(N->values.begin(), N->values.end());
  int temp = N->values.front();
  N->values.erase(N->values.begin());

  S->values.push_back(temp);

  orginize(N->parent);
}

void BPTree::split(Node* N, int k) {
  cout << "splitLeaf\n";
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
  cout << "split\n";
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

int BPTree::max(Node* N) {
  while (!N->leaf) {
    N = N->children.back();
  }
  return N->values.back();
}

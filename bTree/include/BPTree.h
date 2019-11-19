#pragma once
#include <vector>
using namespace std;

class BPTree {
 private:
  class Node {
   public:
    Node(bool isLeaf);
    ~Node();

    vector<Node*> children;
    Node* parent;
    vector<int> values;
    bool leaf;
  };

 public:
  BPTree(int treeOrder = 4);
  ~BPTree();

  bool insert(int key);
  bool find(int key);
  bool remove(int key);

 private:
  void deleteTree(Node* N);
  Node* Sibling(Node* N, bool left);
  void rightShift(Node* N, Node* S, int k);
  void leftShift(Node* N, Node* S, int k);
  void split(Node* N, int k);
  void split(Node* N, Node* C);
  void concatinate(Node* N);
  void orginize(Node* N);
  int max(Node* N);
  Node* root;
  int order;
};

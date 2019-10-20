#pragma once
#include <vector>
using namespace std;

class BPTree {
 private:
  class Node {
   public:
    Node(int treeOrder, bool isLeaf);
    ~Node();

    vector<Node*> children;
    Node* parent;
    vector<int> values;
    int order;
    bool leaf;
  };

 public:
  BPTree(int treeOrder = 4);
  ~BPTree();

  void insert(int num);
  bool find(int num);
  bool remove(int num);

 private:
  Node* insert(int k, Node* T);
  bool leftSibFull(Node* N);
  bool leftSibFull(Node* N);
  Node* root;
  int order;
};

#pragma once
#include <vector>
using namespace std;

class BPTree {
 public:
  // private:
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

  // private:
  Node* insert(Node* T, int k);
  Node* SibRoom(Node* N, bool left);
  void rightShift(Node* N, Node* S, int k);
  void leftShift(Node* N, Node* S, int k);
  Node* root;
  int order;
};

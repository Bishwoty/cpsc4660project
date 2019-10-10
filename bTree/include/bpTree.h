#pragma once
#include <vector>

class bpTree {
private:
  class node {
  public:
    node(int treOrder);
    ~node();

    vector<node> pointers;
    vector<int> values;
  };

public:
  bpTree(int treeOrder = 4);
  ~bpTree();

  void insert(int num);
  bool find(int num);
  bool remove(int num);

  // private:
  node *root;
  int order;
};

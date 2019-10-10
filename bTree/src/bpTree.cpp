#include <bpTree.h>

// NODE ************************************************************************
bpTree::node::node(int treeOrder) {}

bpTree::node::~node() {}
//******************************************************************************

bpTree::bpTree(int treeOrder) : order(treeOrder) { root = nullptr; }

bpTree::~bpTree() {
  if (root) {
    delete root;
    root = nullptr;
  }
}

void bpTree::insert(int num) {}

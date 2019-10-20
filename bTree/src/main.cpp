#include <BPTree.h>
#include <iostream>
using namespace std;

int main() {
  BPTree tree;
  tree.insert(5);
  tree.insert(5);
  tree.insert(2);
  tree.insert(6);
  tree.insert(9);
  tree.root->children.at(0) = new BPTree::Node(4, true);
  tree.root->children.at(0)->parent = tree.root;
  tree.root->children.at(1) = new BPTree::Node(4, true);
  tree.root->children.at(1)->parent = tree.root;
  tree.root->children.at(2) = new BPTree::Node(4, true);
  tree.root->children.at(2)->parent = tree.root;

  tree.root->children.at(1)->values.push_back(0);
  tree.root->children.at(1)->values.push_back(0);
  tree.root->children.at(1)->values.push_back(0);

  tree.root->children.at(0)->values.push_back(0);
  tree.root->children.at(0)->values.push_back(0);
  tree.root->children.at(0)->values.push_back(0);

  cout << tree.SibRoom(tree.root->children.at(2), true) << endl
       << tree.root->children.at(1) << endl
       << tree.root->children.at(0) << endl;
  return 0;
}

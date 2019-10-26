#include <BPTree.h>
#include <iostream>
using namespace std;

int main() {
  BPTree tree;
  for (int i = 0; i < 14; i++) {
    tree.insert(i);
  }

  for (int i = 0; i < tree.root->values.size(); i++) {
    cout << tree.root->values.at(i) << ' ';
  }
  cout << endl << endl;
  for (int j = 0; j < tree.root->children.size(); j++) {
    for (int i = 0; i < tree.root->children.at(j)->values.size(); i++) {
      cout << tree.root->children.at(j)->values.at(i) << ' ';
    }
    cout << endl;
  }
  cout << endl << endl;

  for (int g = 0; g < tree.root->children.size(); g++) {
    for (int j = 0; j < tree.root->children.at(g)->children.size(); j++) {
      for (int i = 0;
           i < tree.root->children.at(g)->children.at(j)->values.size(); i++) {
        cout << tree.root->children.at(g)->children.at(j)->values.at(i) << ' ';
      }
      cout << endl;
    }
  }

  tree.insert(14);
  return 0;
}

#include <BPTree.h>
#include <iostream>
using namespace std;

int main() {
  BPTree tree(4);
  for (int i = 1; i <= 100; i++) {
    tree.insert(i);
  }
  cout << "root: ";

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
    cout << endl;
  }

  for (int i = 1; i <= 100; i++) {
    cout << tree.find(i) << endl;
  }
  /*
    BPTree::Node* left = tree.root->children.at(0);
    BPTree::Node* right = tree.root->children.at(1);

    BPTree::Node* left0 = left->children.at(0);
    BPTree::Node* left1 = left->children.at(1);
    // BPTree::Node* left2 = left->children.at(2);

    BPTree::Node* right0 = right->children.at(0);
    BPTree::Node* right1 = right->children.at(1);
    BPTree::Node* right2 = right->children.at(2);

    cout << right2->parent->values.at(0) << endl;
  */
  // tree.insert(15);
  return 0;
}

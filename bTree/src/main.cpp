#include <BPTree.h>
#include <iostream>
using namespace std;

int main() {
  BPTree tree;
  tree.insert(5);
  tree.insert(5);
  tree.insert(2);
  tree.insert(6);
  tree.insert(5);

  return 0;
}

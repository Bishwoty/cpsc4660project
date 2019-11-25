#include <BPTree.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
using namespace std;
using namespace std::chrono;

int getSeed();

int main() {
  BPTree tree(3);
  while (true) {
    char cmd;
    int key;
    cout << ">>";
    cin >> cmd >> key;
    switch (cmd) {
      case 'i':
        if (tree.insert(key)) {
          cout << key << " inserted" << endl;
        } else {
          cout << key << " already exists" << endl;
        }
        break;
      case 'f':
        if (tree.find(key)) {
          cout << key << " found" << endl;
        } else {
          cout << key << " not found" << endl;
        }
        break;
      case 'r':
        if (tree.remove(key)) {
          cout << key << " removed" << endl;
        } else {
          cout << key << " doesn't exist" << endl;
        }
        break;
    }
  }
  return 0;
}

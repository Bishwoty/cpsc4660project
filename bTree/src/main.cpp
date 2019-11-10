#include <BPTree.h>
#include <algorithm>
#include <array>
#include <ctime>
#include <iostream>
#include <random>
using namespace std;

int main() {
  array<int, 1000> nums;
  BPTree tree(4);
  int seed = 20;  // time(nullptr);

  for (int i = 0; i < nums.size(); i++) {
    nums[i] = i;
  }

  shuffle(nums.begin(), nums.end(), default_random_engine(seed));

  for (int i = 0; i < nums.size(); i++) {
    tree.insert(nums[i]);
    cout << tree.find(nums[i]) << endl;
  }
  /*
    cout << endl << "root: ";

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
             i < tree.root->children.at(g)->children.at(j)->values.size(); i++)
    { cout << tree.root->children.at(g)->children.at(j)->values.at(i) << ' ';
        }
        cout << endl;
      }
      cout << endl;
    }

  for (int i = 0; i < nums.size(); i++) {
    cout << tree.remove(nums[i]) << endl;
    // cout << tree.find(nums[i]) << endl;
  }
*/
  return 0;
}

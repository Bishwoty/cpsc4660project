#include <algorithm>
#include <bucket.h>
#include <iostream>

using namespace std;

Bucket::Bucket(int depth, int size) {
  localDepth = depth;
  bucketSize = size;
}

bool Bucket::search(int key) {
  auto it = std::find(values.begin(), values.end(), key);
  if (it != values.end()) {
    cout << "Value = " << *it << endl;
    return true;
  } else
    return false;
}

int Bucket::insert(int key) {
  bool inserted = search(key);
  if (values.size() == bucketSize)
    return 0;
  if (inserted)
    return 1;
  values.push_back(key);
  return 2;
}

int Bucket::deleteKey(int key) {}

vector<int> Bucket::copy() {
  vector<int> temp(values.begin(), values.end());
  return temp;
}

#include <iostream>
#include <vector>
#pragma once
using namespace std;

class Bucket {
public:
  Bucket(int depth); // localDepth should always be 2.
  bool search(int key);
  int insert(int key);
  int deleteKey(int key);
  int getDepth() { return localDepth; } /// might not need
  int clear() { values.clear(); }

private:
  int localDepth; // localDepth should always be 2.
  vector<int> values;
};

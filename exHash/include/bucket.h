#include <iostream>
#include <vector>
#pragma once
using namespace std;

class Bucket {
public:
  Bucket(int depth, int size);
  bool search(int key);
  int insert(int key);
  int deleteKey(int key);
  int increaseDepth() { return localDepth++; }
  int decreaseDepth() { return localDepth--; }
  int getDepth() { return localDepth; }
  int clear() { values.clear(); }
  vector<int> copy();
  int isFull(void);
  int isEmpty(void);

private:
  int localDepth;
  int bucketSize;
  vector<int> values;
};

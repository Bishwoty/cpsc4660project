#include <iostream>
#include <vector>
#pragma once
using namespace std;

class Bucket {
public:
  Bucket(int depth, int size);
  int getDepth();
  int increaseDepth();
  int decreaseDepth();
  vector<int> copy();
  void clear();

  vector<int> values;
  int localDepth, bucketSizeCap;
};

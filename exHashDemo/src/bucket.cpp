#include <bucket.h>
#include <iostream>

using namespace std;

Bucket::Bucket(int depth, int size) {
  localDepth = depth;
  bucketSizeCap = size;
}

int Bucket::getDepth() { return localDepth; }

int Bucket::increaseDepth() {
  localDepth++;
  return localDepth;
}

int Bucket::decreaseDepth() {
  localDepth--;
  return localDepth;
}

vector<int> Bucket::copy() {
  vector<int> temp(values.begin(), values.end());
  return temp;
}

void Bucket::clear() { values.clear(); }

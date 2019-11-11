#include <iostream>
#include <map>
#pragma once
using namespace std;

class Bucket {
public:
  Bucket(int depth, int size);
  void search(int key);
  int insert(int key, string value);
  int deleteKey(int key);
  int full();
  int empty();
  int getDepth();

private:
  int localDepth, bucketSize;
  map<int, string> values;
};

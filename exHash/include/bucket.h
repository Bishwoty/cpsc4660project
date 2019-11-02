#include <iostream>
#include <map>
#pragma once
using namespace std;

class Bucket {
public:
  Bucket(int depth, int size);
  int insert(int key, string value);
  int deleteKey(int key);
  void search(int key);

  int getDepth();

private:
  int localDepth, bucketSize;
  map<int, string> values;
};

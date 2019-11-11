#include <bucket.h>
#include <iostream>
#include <vector>
#pragma once
using namespace std;

class Address {
public:
  Address(int depth, int size);
  int hash(int n);
  string bucketId(int n);
  void search(int key);
  void insert(int key, string value);
  void deleteKey(int key);

private:
  int globalDepth, bucketSize;
  vector<Bucket *> buckets;
};

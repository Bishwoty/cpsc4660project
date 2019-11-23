#include <bucket.h>
#include <iostream>
#include <vector>
#pragma once
using namespace std;

class Address {
public:
  Address(int depth, int bucketSize);
  int hashFunc(int n);
  bool search(int key, bool fromInsert);
  void insert(int key);
  void deleteKey(int key);

private:
  int globalDepth, bucketSizeCap;
  vector<Bucket *> buckets;

  int pairIndexes(int bucketNo, int depth);
  void grow(); // int bucketNo);
  void shrink();
  void split(int bucketNo);
  void merge(int bucketNo);
  string bucketId(unsigned int bucketNum);
};

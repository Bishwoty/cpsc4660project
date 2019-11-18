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
  void insert(int key);
  void deleteKey(int key);
  void display();

private:
  int globalDepth, bucketSize;
  vector<Bucket *> buckets;
};

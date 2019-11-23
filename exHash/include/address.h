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
  int pairIndexes(int bucketNo, int depth);
  void split(int bucketNo);
  void split(int bucketNo);
  void merge(int bucketNo);
  void grow();
  void shrink();
  void display();

private:
  int globalDepth, bucketSize;
  vector<Bucket *> buckets;
};

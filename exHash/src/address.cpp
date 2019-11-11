#include <address.h>
#include <iostream>

using namespace std;

Address::Address(int depth, int size) {
  globalDepth = depth;
  bucketSize = size;
  for (int i = 0; i < 1 << depth; i++) {
    buckets.push_back(new Bucket(depth, size));
  }
}

int Address::hash(int n) { return n & ((1 << globalDepth) - 1); }

string Address::bucketId(int n) {
  int d;
  string s;

  d = buckets[n]->getDepth();
  s = "";

  while (n > 0 && d > 0) {
    s = (n % 2 == 0 ? "0" : "1") + s;
    n /= 2;
    d--;
  }
  while (d > 0) {
    s = "0" + s;
    d--;
  }
  return s;
}

void Address::search(int key) {
  int bucketNo = hash(key);
  buckets[bucketNo]->search(key);
  cout << "Finding key:" << key << " in bucket " << bucketId(bucketNo) << endl;
}

void Address::insert(int key, string value) {
  int bucketNo = hash(key);
  buckets[bucketNo]->insert(key, value);
}

void Address::deleteKey(int key) {
  int bucketNo = hash(key);
  buckets[bucketNo]->deleteKey(key);
  cout << "Deleted key: " << key << " from bucket " << bucketId(bucketNo)
       << endl;
}

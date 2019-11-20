#include <address.h>
#include <iostream>

using namespace std;

Address::Address(int depth, int size) {
  globalDepth = depth;
  bucketSize = size;
  for (int i = 0; i < 1 << depth; i++) {
    buckets.push_back(new Bucket(bucketSize, bucketSize));
  }
}

int Address::hash(int n) { return ((n % 64) >> (6 - globalDepth)); }

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
  bool inserted = buckets[bucketNo]->search(key);
  if (inserted == true)
    cout << "Found key:" << key << " in bucket: " << bucketId(bucketNo) << endl;
  else
    cout << "This key does not exist. ???" << endl;
}

void Address::insert(int key) {
  int bucketNo = hash(key);
  int status = buckets[bucketNo]->insert(key);
  cout << "Status: " << status << endl;
  if (status == 0) {
    cout << "Bucket full, splitting...";
    split(bucketNo);
    insert(key);
  } else if (status == 1) {
    cout << "Key already in bucket." << endl;
  } else
    cout << "Inserted key: " << key << " into bucket: " << bucketId(bucketNo)
         << endl;
}

void Address::deleteKey(int key) {
  int bucketNo = hash(key);
  buckets[bucketNo]->deleteKey(key);
  cout << "Deleted key: " << key << " from bucket " << bucketId(bucketNo)
       << endl;
}

int Address::pairIndexes(int bucketNo, int depth) {
  return bucketNo ^ (1 << (depth - 1));
}

void Address::split(int bucketNo) {
  int localDepth, pairIndex;
  int indexDiff, addressSize;
  vector<int> temp;

  localDepth = buckets[bucketNo]->increaseDepth();
  if (localDepth > globalDepth)
    grow();
  pairIndex = pairIndexes(bucketNo, localDepth);
  buckets[pairIndex] = new Bucket(localDepth, bucketSize);
  temp = buckets[bucketNo]->copy();
  buckets[bucketNo]->clear();

  indexDiff = 1 << localDepth;
  addressSize = 1 << globalDepth;
  int i;
  for (i = pairIndex - indexDiff; i >= 0; i -= indexDiff)
    buckets[i] = buckets[pairIndex];
  for (i = pairIndex + indexDiff; i < addressSize; i += indexDiff)
    buckets[i] = buckets[pairIndex];
  for (auto it = temp.begin(); it != temp.end(); it++)
    insert(*it);
}

void Address::grow() {
  for (int i = 0; i < 1 << globalDepth; i++)
    buckets.push_back(buckets[i]);
  globalDepth++;
}

void Address::shrink() {
  // buckets.pop_back(buckets[i]);
  globalDepth--;
}

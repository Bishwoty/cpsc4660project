#include <address.h>
#include <iostream>

using namespace std;

Address::Address(int depth, int size) {
  globalDepth = depth;
  bucketSize = size;
  for (int i = 0; i < 1 << depth; i++) {
    buckets.push_back(new Bucket(bucketSize));
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
    // split(bucketNo);
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

void Address::split(int bucketNo) {
  // if () // something
  // grow();
}

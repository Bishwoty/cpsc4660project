#include <bucket.h>
#include <iostream>

using namespace std;

Bucket::Bucket(int depth, int size) {
  localDepth = depth;
  bucketSize = size;
}

void Bucket::search(int key) {
  map<int, string>::iterator i;
  i = values.find(key);
  if (i != values.end())
    cout << " Value: " << i->second << endl;
  else
    cout << "Key does not exist." << endl;
}

int Bucket::insert(int key) {
  values.find(key);
  /*if (i != values.end())
    return -1;
  if (full())
    return 0;
  values[key] = value;
  return 1;*/
  return values.find(key);
}

int Bucket::deleteKey(int key) {
  map<int, string>::iterator i;
  i = values.find(key);
  if (i != values.end()) {
    values.erase(i);
    return 1;
  } else {
    cout << "Key does not exist." << endl;
    return 0;
  }
}

int Bucket::getDepth() { return localDepth; }

int Bucket::full() {
  if (values.size() == bucketSize)
    return 1;
  else
    return 0;
}

int Bucket::empty() {
  if (values.size() == 0)
    return 1;
  else
    return 0;
}

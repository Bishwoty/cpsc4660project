#include <bucket.h>
#include <iostream>

using namespace std;

Bucket::Bucket(int depth, int size) {
  localDepth = depth;
  bucketSize = size;
}
int Bucket::insert(int key, string value) {}
int Bucket::deleteKey(int key) {}
void Bucket::search(int key) {}
int Bucket::getDepth() {}

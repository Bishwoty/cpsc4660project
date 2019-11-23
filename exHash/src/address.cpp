#include <address.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <iostream>

using namespace std;

Address::Address(int depth, int bucketSize) {
  globalDepth = depth;
  bucketSizeCap = bucketSize;
  for (int i = 0; i < 1 << depth; i++) {
    buckets.push_back(new Bucket(depth, bucketSizeCap));
  }
}

int Address::hashFunc(int n) { return ((n % 64) >> (6 - globalDepth)); }

bool Address::search(int key, bool displayMessages) {
  int bucketNo = hashFunc(key);
  vector<int>::iterator it;
  it = find(buckets[bucketNo]->values.begin(), buckets[bucketNo]->values.end(),
            key);
  if (it != buckets[bucketNo]->values.end()) {
    if (displayMessages == true)
      cout << "Found key<" << key << ">in bucket<" << bucketId(bucketNo) << ">"
           << endl;
    return true;
  } else {
    if (displayMessages == true)
      cout << "The key<" << key << ">does not exist." << endl;
    return false;
  }
}

void Address::insert(int key) {
  int bucketNo = hashFunc(key);
  bool displayMessages = false;
  bool isFound = search(key, displayMessages);
  if (isFound == false) {
    if (buckets[bucketNo]->values.size() < bucketSizeCap) {
      buckets[bucketNo]->values.push_back(key);
      cout << "Inserted key<" << key << ">in bucket<" << bucketId(bucketNo)
           << ">" << endl;
    } else {
      split(bucketNo);
      insert(key);
      cout << "Inserted key<" << key << ">in bucket<" << bucketId(bucketNo)
           << ">" << endl;
    }
  } else
    cout << "The key<" << key << ">already exists." << endl;
}

void Address::deleteKey(int key) {
  int bucketNo = hashFunc(key);
  bool displayMessages = false;
  bool isFound = search(key, displayMessages);
  if (isFound == true) {
    vector<int>::iterator it;
    it = find(buckets[bucketNo]->values.begin(),
              buckets[bucketNo]->values.end(), key);
    if (it != buckets[bucketNo]->values.end()) {
      buckets[bucketNo]->values.erase(it);
      cout << "Deleted key<" << key << ">from bucket<" << bucketId(bucketNo)
           << ">" << endl;
    }
    merge(bucketNo); // Try to merge buckets
    shrink();        // Try to shrink address
  } else
    cout << "The key<" << key << ">does not exist." << endl;
}

int Address::pairIndexes(int bucketNo, int depth) {
  return bucketNo ^ (1 << (depth - 1));
}

void Address::grow() { // int bucketNo) {
  for (int i = 0; i < (1 << globalDepth); i++)
    buckets.push_back(buckets[i]);
  globalDepth++;
  /*
    for (int i = 0; i < (1 << globalDepth); ++i) {
      if (i != bucketNo)
        buckets[bucketNo]->values[i ^ (1 << globalDepth)] =
            buckets[bucketNo]->values[i];
    }
    globalDepth++;*/
}
void Address::shrink() {
  int i, flag = 1;
  for (i = 0; i < buckets.size(); i++) {
    if (buckets[i]->getDepth() == globalDepth) {
      flag = 0;
      return;
    }
  }
  globalDepth--;
  for (i = 0; i < 1 << globalDepth; i++)
    buckets.pop_back();
}

void Address::split(int bucketNo) {
  cout << "Splitting bucket<" << bucketId(bucketNo) << ">";

  int localDepth, pairIndex, indexDiff, addressSize;
  vector<int> temp;
  vector<int>::iterator it;

  localDepth = buckets[bucketNo]->increaseDepth();
  if (localDepth > globalDepth)
    grow();
  pairIndex = pairIndexes(bucketNo, localDepth);
  buckets[pairIndex] = new Bucket(localDepth, bucketSizeCap);
  temp = buckets[bucketNo]->copy();
  buckets[bucketNo]->clear();
  indexDiff = 1 << localDepth;
  addressSize = 1 << globalDepth;
  int i;
  for (i = pairIndex - indexDiff; i >= 0; i -= indexDiff)
    buckets[i] = buckets[pairIndex];
  for (i = pairIndex + indexDiff; i < addressSize; i += indexDiff)
    buckets[i] = buckets[pairIndex];
  for (it = temp.begin(); it != temp.end(); it++)
    insert(*it);

  /*
    Bucket *newBucket = new Bucket(globalDepth, bucketSizeCap);
    vector<int> temp;
    temp = buckets[bucketNo]->copy();
    buckets[bucketNo]->clear();

    if (buckets[bucketNo]->localDepth == globalDepth) { // Split and expand
      buckets[bucketNo]->values[bucketNo ^ (1 << globalDepth)] = newBucket;
      if (globalDepth != 0)
        grow(bucketNo);
      newBucket->localDepth = ++buckets[bucketNo]->localDepth;
      for (auto i : temp) {
        int bNo = hashFunc(i);
        buckets[bucketNo]->values[bNo].push_back(i);
      }
    } else { // Just split

      int k = bucketNo & (1 << (buckets[bucketNo]->localDepth) - 1);
      vector<int> indices;
      for (int i = 0; i < (1 << globalDepth); ++i) {
        int last = i & (1 << (buckets[bucketNo]->localDepth)) - 1;
        if (last == k)
          indices.push_back(i);
      }
      newBucket->localDepth = ++buckets[bucketNo]->localDepth;
      for (int i = indices.size() / 2; i < indices.size(); ++i)
        buckets[bucketNo]->values[indices[i]] = newBucket;

      for (auto i : temp) {
        int bNo = hashFunc(i);
        buckets[bucketNo]->values[bNo].push_back(i);
      }
    }*/
}

void Address::merge(int bucketNo) {
  int localDepth, pairIndex, indexDiff, addressSize, i;

  localDepth = buckets[bucketNo]->getDepth();
  pairIndex = pairIndexes(bucketNo, localDepth);
  indexDiff = 1 << localDepth;
  addressSize = 1 << globalDepth;

  if (buckets[pairIndex]->getDepth() == localDepth) {
    buckets[pairIndex]->decreaseDepth();
    delete (buckets[bucketNo]);
    buckets[bucketNo] = buckets[pairIndex];
    for (i = bucketNo - indexDiff; i >= 0; i -= indexDiff)
      buckets[i] = buckets[pairIndex];
    for (i = bucketNo + indexDiff; i < addressSize; i += indexDiff)
      buckets[i] = buckets[pairIndex];
  }
}

string Address::bucketId(unsigned int bucketNum) {
  // string binary = std::bitset<8>(bucketNum).to_string(); // Convert to binary
  // return binary;
  string s = "";
  for (int i = (7 >> (6 - globalDepth)); i >= 0; i--) {
    int k = bucketNum >> i;
    if (k & 1)
      s += "1";
    else
      s += "0";
  }
  return s;
}

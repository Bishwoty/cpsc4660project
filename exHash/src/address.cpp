#include <address.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <cmath>
#include <iostream>

using namespace std;

Address::Address(int depth, int bucketSize) {
  globalDepth = depth;
  bucketSizeCap = bucketSize;
  for (int i = 0; i < 1 << depth; i++) {
    buckets.push_back(new Bucket(depth, bucketSizeCap));
  }
}

void Address::display() {
  for (int i = 0; i < (1 << globalDepth); ++i) {
    cout << i << " -> ";
    if (buckets[i]->localDepth == globalDepth ||
        i % static_cast<int>(pow(2, globalDepth - buckets[i]->localDepth)) ==
            0) {
      for (int j = 0; j < buckets[i]->values.size(); j++) {
        cout << buckets[i]->values[j] << ' ';
      }
    } else {
      cout << '^';
    }
    cout << endl;
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
  cout << "BucketNo<" << bucketNo << ">";
  bool displayMessages = false;
  bool isFound = search(key, displayMessages);
  if (isFound == false) {
    if (buckets[bucketNo]->values.size() < bucketSizeCap) {
      buckets[bucketNo]->values.push_back(key);
      cout << "Inserted key<" << key << ">in bucket<" << bucketId(bucketNo)
           << ">" << endl;
    } else if (buckets[bucketNo]->localDepth == globalDepth) {
      grow();
      insert(key);
    } else {
      split(bucketNo);
      insert(key);
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

void Address::grow() {
  int size = buckets.size();
  for (int i = 0; i < size; i++)
    buckets.push_back(nullptr);
  for (int i = buckets.size() - 1; i >= 0; i--) {
    buckets[i] = buckets[i / 2];
  }

  globalDepth++;
  cout << "Global depth<" << globalDepth << ">" << endl;
}

void Address::shrink() {
  for (int i = 0; i < buckets.size(); i++) {
    if (buckets[i]->getDepth() == globalDepth)
      return;
  }
  globalDepth--;
  for (int j = 0; j < 1 << globalDepth; j++)
    buckets.pop_back();
}

void Address::split(int bucketNo) {
  cout << "Splitting bucket<" << bucketId(bucketNo) << "> ";
  int depthDiff = globalDepth - buckets[bucketNo]->localDepth;
  vector<int> temp;
  temp = buckets[bucketNo]->copy();
  buckets[bucketNo]->clear();
  bucketNo -= bucketNo % static_cast<int>(pow(2, depthDiff));
  buckets[bucketNo]->increaseDepth();
  buckets[bucketNo + 1] =
      new Bucket(buckets[bucketNo]->localDepth, bucketSizeCap);
  for (int i = 0; i < temp.size(); i++) {
    insert(temp[i]);
  }

  // int localDepth, pairIndex, indexDiff, addressSize;
  // vector<int> temp;
  // vector<int>::iterator it;
  //
  // localDepth = buckets[bucketNo]->increaseDepth();
  // if (localDepth > globalDepth)
  //   grow();
  // pairIndex = pairIndexes(bucketNo, localDepth);
  // buckets[pairIndex] = new Bucket(localDepth, bucketSizeCap);
  // temp = buckets[bucketNo]->copy();
  // buckets[bucketNo]->clear();
  // indexDiff = 1 << localDepth;
  // addressSize = 1 << globalDepth;
  // for (int i = pairIndex - indexDiff; i >= 0; i -= indexDiff)
  //   buckets[i] = buckets[pairIndex];
  // for (int j = pairIndex + indexDiff; j < addressSize; j += indexDiff)
  //   buckets[j] = buckets[pairIndex];
  // for (it = temp.begin(); it != temp.end(); it++)
  //   insert(*it);

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
  int localDepth, pairIndex, indexDiff, addressSize;

  localDepth = buckets[bucketNo]->getDepth();
  pairIndex = pairIndexes(bucketNo, localDepth);
  indexDiff = 1 << localDepth;
  addressSize = 1 << globalDepth;

  if (buckets[pairIndex]->getDepth() == localDepth) {
    buckets[pairIndex]->decreaseDepth();
    delete (buckets[bucketNo]);
    buckets[bucketNo] = buckets[pairIndex];
    for (int i = bucketNo - indexDiff; i >= 0; i -= indexDiff)
      buckets[i] = buckets[pairIndex];
    for (int j = bucketNo + indexDiff; j < addressSize; j += indexDiff)
      buckets[j] = buckets[pairIndex];
  }
}

string Address::bucketId(int bucketNum) {
  int depth = buckets[bucketNum]->getDepth();
  string localAddress = "";
  while (bucketNum > 0 && depth > 0) {
    localAddress = (bucketNum % 2 == 0 ? "0" : "1") + localAddress;
    bucketNum /= 2;
    depth--;
  }
  while (depth > 0) {
    localAddress = "0" + localAddress;
    depth--;
  }
  return localAddress;
}

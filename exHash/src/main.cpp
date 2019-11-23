#include <address.h>
#include <bits/stdc++.h>
#include <bucket.h>
#include <iostream>

using namespace std;

int main() {
  int size, key;
  string operation;

  Address a(1, 2); // globalDepth = 1, bucketSize = 2
  cout << ">> i <key>" << endl;
  cout << ">> d <key>" << endl;
  cout << ">> s <key>" << endl;
  cout << ">> exit" << endl;

  do {
    cout << ">> ";
    cin >> operation;

    if (operation == "i") {
      cin >> key;
      a.insert(key);
    } else if (operation == "d") {
      cin >> key;
      a.deleteKey(key);
    } else if (operation == "s") {
      cin >> key;
      a.search(key);
    }

  } while (operation != "exit");

  return 0;
}

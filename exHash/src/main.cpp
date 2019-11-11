#include <address.h>
#include <bits/stdc++.h>
#include <bucket.h>
#include <iostream>

using namespace std;

int main() {
  int size, key;
  string operation, value;

  cout << "Hash function is % 32" << endl;
  cout << "Enter bucket size: ";
  cin >> size;
  cout << "Enter operation(s, i, d) or exit: "; // Search, Insert or Delete
  cin >> operation;

  Address a(1, size);

  do {
    if (operation == "s") { // Search
      cin >> key;
      a.search(key);
    } else if (operation == "i") { // Insert
      cin >> key >> value;
      a.insert(key, value);
    } else if (operation == "d") { // Delete
      cin >> key;
      a.deleteKey(key);
    }
  } while (operation != "exit");

  return 0;
}

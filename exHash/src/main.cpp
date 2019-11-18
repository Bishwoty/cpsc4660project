#include <address.h>
#include <bits/stdc++.h>
#include <bucket.h>
#include <iostream>

using namespace std;

int main() {
  int size, key;
  string operation, value;

  do {
    cout << "Enter value: ";
    cin >> key;
    Address a(1, 2); // globalDepth = 1, bucketSize = 2
    a.insert(key);
    cout << "Insert " << key << " - " << a.display() << endl;
  } while (key != 0);

  /*
  cout << "Hash function is % 64, global depth = 1, bucket size = 2." << endl;
  cout << "Enter operation(s <value>, i <value>, d <value>) or exit: "; //
  Search, Insert or Delete cin >> operation >> key;

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
  */
  return 0;
}

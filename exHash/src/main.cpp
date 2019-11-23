#include <address.h>
#include <bits/stdc++.h>
#include <bucket.h>
#include <iostream>

using namespace std;

int main() {
  int key;
  string choice;
  Address a(1, 2);

  cout << "<i> <key>" << endl;
  cout << "<d> <key>" << endl;
  cout << "<s> <key>" << endl;
  cout << "exit" << endl;

  do {
    cout << endl;
    cout << ">>";
    cin >> choice;
    if (choice == "i") {
      cin >> key;
      a.insert(key);
    } else if (choice == "d") {
      cin >> key;
      a.deleteKey(key);
    } else if (choice == "s") {
      cin >> key;
      bool displayMessages = true;
      a.search(key, displayMessages);
    }
  } while (choice != "exit");
  return 0;
}

#include <address.h>
#include <algorithm>
#include <array>
#include <bucket.h>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

int main() {
  // ~~~~~~~~~~~~~~ Extendible hashing program ~~~~~~~~~~~~~~~~~~

  int key, seed;
  string choice;
  Address a(0, 2);

  cout << "<i> <key>" << endl;
  cout << "<d> <key>" << endl;
  cout << "<s> <key>" << endl;
  cout << "display" << endl;
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
      a.search(key);
    } else if (choice == "display") {
      a.display();
    }
    a.display();
  } while (choice != "exit");

  // ~~~~~~~~~~~~~~ Extendible hashing program end ~~~~~~~~~~~~~~

  return 0;
}

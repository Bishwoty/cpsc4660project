#include <address.h>
#include <algorithm>
#include <array>
#include <bucket.h>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

int getSeed();

int main() {

  int seed = 1; // getSeed();
  array<int, 100> nums;
  Address a(0, 20);
  array<nanoseconds, 100> insertTimes;
  array<nanoseconds, 100> findTimes;
  // array<nanoseconds, 100> removeTimes;
  time_point<steady_clock> start, end;

  for (int i = 0; i < nums.size(); i++)
    nums[i] = i;

  shuffle(nums.begin(), nums.end(), default_random_engine(seed));
  for (int i = 0; i < nums.size(); i++) {
    start = steady_clock::now();
    a.insert(nums[i]);
    end = steady_clock::now();
    insertTimes[i] = end - start;
    start = steady_clock::now();
    a.search(nums[i]);
    end = steady_clock::now();
    findTimes[i] = end - start;
  }
  for (int i = 0; i < insertTimes.size(); i++)
    cout << i << '\t' << insertTimes[i].count() << '\t' << findTimes[i].count()
         << endl;
  return 0;
}

int getSeed() {
  cout << "Enter seed: ";
  int seed;
  cin >> seed;
  return seed;
}

#include <BPTree.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
using namespace std;
using namespace std::chrono;

int getSeed();

int main() {
  int seed = getSeed();
  array<int, 1000> nums;
  BPTree tree(3);
  array<nanoseconds, 1000> insertTimes;
  array<nanoseconds, 1000> findTimes;
  array<nanoseconds, 1000> removeTimes;
  time_point<steady_clock> start, end;

  for (int i = 0; i < nums.size(); i++) {
    nums[i] = i;
  }

  shuffle(nums.begin(), nums.end(), default_random_engine(seed));

  for (int i = 0; i < nums.size(); i++) {
    start = steady_clock::now();
    tree.insert(nums[i]);
    end = steady_clock::now();
    insertTimes[i] = end - start;
    start = steady_clock::now();
    tree.find(nums[i]);
    end = steady_clock::now();
    findTimes[i] = end - start;
  }

  shuffle(nums.begin(), nums.end(), default_random_engine(seed));

  for (int i = 0; i < nums.size(); i++) {
    start = steady_clock::now();
    tree.remove(nums[i]);
    end = steady_clock::now();
    removeTimes[removeTimes.size() - 1 - i] = end - start;
  }
  for (int i = 0; i < insertTimes.size(); i++) {
    cout << i << '\t' << insertTimes[i].count() << '\t' << findTimes[i].count()
         << '\t' << removeTimes[i].count() << endl;
  }
  return 0;
}

int getSeed() {
  cout << "Enter seed: ";
  int seed;
  cin >> seed;
  return seed;
}

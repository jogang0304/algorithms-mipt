#include <algorithm>
#include <iostream>
#include <vector>

long long FindLightME(std::vector<long long>& coordinates, long long position) {
  long long left = -1;
  long long right = coordinates.size() - 1;
  while (right - left > 1) {
    long long middle = (left + right) / 2;
    if (coordinates[middle] >= position) {
      right = middle;
    } else {
      left = middle;
    }
  }
  return right;
}

long long HowManyLightsNeeded(std::vector<long long>& coordinates,
                              long long light_area, long long max_lights) {
  long long ans = 0;
  long long cur_pos = coordinates[0];
  while (cur_pos <= (coordinates[coordinates.size() - 1])) {
    cur_pos = coordinates[FindLightME(coordinates, cur_pos)] + light_area + 1;
    ++ans;
    if (ans > max_lights) {
      break;
    }
  }
  return ans;
}

int main() {
  long long amount_of_receivers;
  long long max_lights;
  std::cin >> amount_of_receivers >> max_lights;
  std::vector<long long> coordinates(amount_of_receivers);
  for (auto& receiver : coordinates) {
    std::cin >> receiver;
  }
  std::sort(coordinates.begin(), coordinates.end());

  long long left = -1;
  long long right = coordinates[coordinates.size() - 1] - coordinates[0] + 1;
  while (right - left > 1) {
    long long middle = (left + right) / 2;
    if (HowManyLightsNeeded(coordinates, middle, max_lights) <= max_lights) {
      right = middle;
    } else {
      left = middle;
    }
  }
  if (HowManyLightsNeeded(coordinates, left, max_lights) <= max_lights) {
    right = left;
  }
  std::cout << right;
}
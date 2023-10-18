#include <iostream>
#include <vector>

int main() {
  int amount_of_points;
  std::cin >> amount_of_points;
  std::vector<int> weights(amount_of_points);
  for (auto& element : weights) {
    std::cin >> element;
  }
  int points_for_second = amount_of_points / 2;
  std::vector<int> sums(
      amount_of_points);  // sum of point_for_second elements starting from i
  sums[0] = 0;
  for (int i = 0; i < points_for_second; ++i) {
    sums[0] += weights[i];
  }
  for (int i = 1; i < amount_of_points; ++i) {
    sums[i] = sums[i - 1] +
              weights[(i * points_for_second) % amount_of_points] -
              weights[i - 1];
  }
  
}
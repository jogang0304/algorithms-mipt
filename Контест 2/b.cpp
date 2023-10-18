#include <algorithm>
#include <iostream>
#include <vector>

// max на [left, right)
long long MaxArea(std::vector<int>& heights, size_t left, size_t right) {
  if (right - left == 1) {
    return heights[left];
  }
  if (right - left < 1) {
    return -1;
  }
  int min_height = heights[left];
  int min_height_index = left;
  for (size_t i = left + 1; i < right; ++i) {
    if ((heights[i] < min_height) ||
        (heights[i] == min_height &&
         (abs(static_cast<int>(i) - static_cast<int>(heights.size()) / 2) <
          abs(static_cast<int>(min_height_index) -
              static_cast<int>(heights.size()) / 2)))) {
      min_height = heights[i];
      min_height_index = i;
    }
  }
  long long answer = (right - left) * min_height;
  auto left_max = MaxArea(heights, left, min_height_index);
  auto right_max = MaxArea(heights, min_height_index + 1, right);
  answer = std::max(answer, std::max(left_max, right_max));
  return answer;
}

int main() {
  int amount_of_heights;
  std::cin >> amount_of_heights;
  std::vector<int> heights(amount_of_heights);
  for (auto& height : heights) {
    std::cin >> height;
  }
  std::cout << MaxArea(heights, 0, heights.size());
}
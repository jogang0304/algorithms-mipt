#include <iostream>
#include <vector>

const long long kInf = 2'000'000'000;

int main() {
  int amount_of_heights;
  std::cin >> amount_of_heights;
  std::vector<int> heights(amount_of_heights);
  for (auto& height : heights) {
    std::cin >> height;
  }
  long long max_area = 0;
  for (size_t start_height = 0; start_height < heights.size(); ++start_height) {
    long long cur_min_height = kInf;
    long long cur_max_area = 0;
    for (size_t i = start_height;
         i < heights.size() &&
         (cur_min_height *
              (static_cast<long long>(heights.size() - start_height)) >
          max_area);
         ++i) {
      long long height = heights[i];
      if (height < cur_min_height) {
        cur_min_height = height;
      }
      long long cur_area = cur_min_height * (i - start_height + 1);
      if (cur_area > cur_max_area) {
        cur_max_area = cur_area;
      }
    }
    if (cur_max_area > max_area) {
      max_area = cur_max_area;
    }
  }
  std::cout << max_area << std::endl;
}
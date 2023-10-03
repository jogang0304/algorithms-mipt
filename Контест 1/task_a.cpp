#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

int main() {
  int amount_of_candidates;
  std::cin >> amount_of_candidates;
  std::vector<int> speeds(amount_of_candidates);
  for (auto& speed : speeds) {
    std::cin >> speed;
  }

  std::vector<int> prefix_min(speeds.size() + 1);
  prefix_min[0] = kInf;
  for (size_t i = 1; i < prefix_min.size(); ++i) {
    prefix_min[i] = std::min(prefix_min[i - 1], speeds[i - 1]);
  }
  std::vector<int> suffix_min(speeds.size() + 1);
  suffix_min[speeds.size()] = speeds[speeds.size() - 1];
  for (size_t i = speeds.size() - 1; i > 0; --i) {
    suffix_min[i] = std::min(suffix_min[i + 1], speeds[i - 1]);
  }

  size_t amount_of_questions;
  std::cin >> amount_of_questions;
  for (size_t i = 0; i < amount_of_questions; ++i) {
    size_t left;
    size_t right;
    std::cin >> left >> right;
    int answer = std::min(prefix_min[left], suffix_min[right]);
    std::cout << answer << "\n";
  }
}
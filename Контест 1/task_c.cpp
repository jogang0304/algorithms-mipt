#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

std::vector<std::vector<int>> GetCombinations(
    std::vector<std::vector<int>>& a_shelves,
    std::vector<std::vector<int>>& b_shelves) {
  std::vector<std::vector<int>> combinations(
      a_shelves.size(), std::vector<int>(b_shelves.size()));
  for (size_t i = 0; i < a_shelves.size(); ++i) {
    for (size_t j = 0; j < b_shelves.size(); ++j) {
      int cur_min = kInf;
      int cur_min_k = -1;
      for (size_t k = 0; k < a_shelves[i].size(); ++k) {
        int cur_max = std::max(a_shelves[i][k], b_shelves[j][k]);
        if (cur_max < cur_min) {
          cur_min = cur_max;
          cur_min_k = k + 1;
        }
      }
      combinations[i][j] = cur_min_k;
    }
  }
  return combinations;
}

int main() {
  int a_size;
  int b_size;
  int shelf_size;
  std::cin >> a_size >> b_size >> shelf_size;
  std::vector<std::vector<int>> a_shelves(a_size, std::vector<int>(shelf_size));
  for (int i = 0; i < a_size; ++i) {
    for (int j = 0; j < shelf_size; ++j) {
      std::cin >> a_shelves[i][j];
    }
  }
  std::vector<std::vector<int>> b_shelves(b_size, std::vector<int>(shelf_size));
  for (int i = 0; i < b_size; ++i) {
    for (int j = 0; j < shelf_size; ++j) {
      std::cin >> b_shelves[i][j];
    }
  }
  auto combinations = GetCombinations(a_shelves, b_shelves);

  int amount_of_questions;
  std::cin >> amount_of_questions;
  for (int i = 0; i < amount_of_questions; ++i) {
    int a_index;
    int b_index;
    std::cin >> a_index >> b_index;
    std::cout << combinations[a_index - 1][b_index - 1] << "\n";
  }
}
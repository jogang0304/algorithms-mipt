#include <cmath>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

const int kInf = 1e9;

struct Cell {
  int sum = kInf;
  int sum_to_last_base = kInf;
  std::vector<int> bases = {};
};

std::pair<int, int> RecountForOneBase(int base,
                                      const std::vector<int>& coordinates) {
  int answer = 0;
  int answer_to_last_base = 0;
  for (int i = 0; i < static_cast<int>(coordinates.size()); ++i) {
    answer += std::abs(coordinates[base] - coordinates[i]);
    if (i <= base) {
      answer_to_last_base += std::abs(coordinates[base] - coordinates[i]);
    }
  }
  return {answer, answer_to_last_base};
}

std::pair<int, int> Recount(const std::vector<int>& bases,
                            const std::vector<int>& coordinates,
                            int sum_without_last) {
  if (bases.size() == 1) {
    return RecountForOneBase(bases[0], coordinates);
  }
  int cur_place_index = bases.size() - 2;
  int answer = sum_without_last;
  int answer_to_last_base = sum_without_last;
  for (int i = bases[bases.size() - 2] + 1;
       i < static_cast<int>(coordinates.size()); ++i) {
    while (i > bases[cur_place_index] &&
           cur_place_index < static_cast<int>(bases.size()) - 1 &&
           std::abs(coordinates[bases[cur_place_index]] - coordinates[i]) >
               std::abs(coordinates[bases[cur_place_index + 1]] -
                        coordinates[i])) {
      ++cur_place_index;
    }
    answer += std::abs(coordinates[bases[cur_place_index]] - coordinates[i]);
    if (i < bases[bases.size() - 1]) {
      answer_to_last_base +=
          std::abs(coordinates[bases[cur_place_index]] - coordinates[i]);
    }
  }
  return {answer, answer_to_last_base};
}

int main() {
  // dp[i][j] - min sum using i bases if last base is j house
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int amount_of_houses;
  int amount_of_bases;
  std::cin >> amount_of_houses >> amount_of_bases;
  std::vector<int> coordinates(amount_of_houses);
  for (int i = 0; i < amount_of_houses; ++i) {
    std::cin >> coordinates[i];
  }
  std::vector<std::vector<Cell>> dp(amount_of_bases + 1,
                                    std::vector<Cell>(amount_of_houses));
  for (int i = 1; i < static_cast<int>(dp.size()); ++i) {
    for (int j = 0; j < static_cast<int>(coordinates.size()); ++j) {
      if (i > j + 1) {
        dp[i][j].sum = kInf;
        continue;
      }
      for (int k = i - 1; k < j; ++k) {
        // std::vector<int> bases = dp[i - 1][k].bases;
        // bases.push_back(j);
        dp[i - 1][k].bases.push_back(j);
        auto rec = Recount(dp[i - 1][k].bases, coordinates,
                           dp[i - 1][k].sum_to_last_base);
        if (rec.first < dp[i][j].sum) {
          dp[i][j].sum = rec.first;
          dp[i][j].sum_to_last_base = rec.second;
          dp[i][j].bases = dp[i - 1][k].bases;
        }
        dp[i - 1][k].bases.pop_back();
      }
    }
  }
  Cell best_cell;
  for (int i = amount_of_bases - 1; i < amount_of_houses; ++i) {
    if (dp[amount_of_bases][i].sum < best_cell.sum) {
      best_cell = dp[amount_of_bases][i];
    }
  }
  std::cout << best_cell.sum << "\n";
  for (auto place : best_cell.bases) {
    std::cout << coordinates[place] << " ";
  }
}
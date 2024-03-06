#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

const int kInf = 1e9;

struct Cell {
  int sum = kInf;
  std::vector<int> bases = {};
};

int Recount(const std::vector<int>& bases, const std::vector<int>& coordinates,
            int right) {
  int cur_place_index = 0;
  int answer = 0;
  for (int i = 0; i < std::min(static_cast<int>(coordinates.size()), right + 1);
       ++i) {
    while (i > bases[cur_place_index] &&
           cur_place_index < static_cast<int>(bases.size()) - 1 &&
           std::abs(coordinates[bases[cur_place_index]] - coordinates[i]) >
               std::abs(coordinates[bases[cur_place_index + 1]] -
                        coordinates[i])) {
      ++cur_place_index;
    }
    answer += std::abs(coordinates[bases[cur_place_index]] - coordinates[i]);
  }
  return answer;
}

int main() {
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
      if (i == j + 1) {
        dp[i][j].sum = 0;
        for (int k = 0; k <= std::min(j, i - 1); ++k) {
          dp[i][j].bases.push_back(k);
        }
        continue;
      }
      int recount1 = Recount(dp[i][j - 1].bases, coordinates, j);
      dp[i][j].sum = recount1;
      dp[i][j].bases = dp[i][j - 1].bases;
      for (int k = i - 1; k < j; ++k) {
        std::vector<int> places2 = dp[i - 1][k].bases;
        places2.push_back(k + 1);
        int recount2 = Recount(places2, coordinates, j);
        if (dp[i][j].sum > recount2) {
          dp[i][j].sum = recount2;
          dp[i][j].bases = places2;
        }
      }
    }
  }
  Cell best_cell = dp[amount_of_bases][coordinates.size() - 1];
  std::cout << best_cell.sum << std::endl;
  for (auto place : best_cell.bases) {
    std::cout << coordinates[place] << " ";
  }
}
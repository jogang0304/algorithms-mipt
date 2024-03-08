#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  int power;
  std::cin >> power;
  // dp[i][j] - how much with power i, if biggest element is j
  std::vector<std::vector<int64_t>> dp(power + 1,
                                       std::vector<int64_t>(power + 1, 0));
  std::vector<std::vector<int64_t>> pref_sum(
      power + 1, std::vector<int64_t>(power + 1, 0));
  for (int i = 1; i <= power; ++i) {
    for (int j = 1; j <= i; ++j) {
      if (i == j) {
        dp[i][j] = 1;
        continue;
      }
      dp[i][j] = pref_sum[i - j][j / 2];
    }
    for (int j = 1; j <= power; ++j) {
      pref_sum[i][j] = pref_sum[i][j - 1] + dp[i][j];
    }
  }
  auto answer = pref_sum[power][power];
  std::cout << answer;
}
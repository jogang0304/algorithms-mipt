#include <algorithm>
#include <iostream>
#include <vector>

struct Input {
  int max_floor = 0;
  int amount_of_helicopters = 0;
};

Input ReadInput() {
  Input input;
  std::cin >> input.max_floor >> input.amount_of_helicopters;
  return input;
}

bool SimplestCase(const Input& input) {
  if (input.amount_of_helicopters == 0) {
    if (input.max_floor == 1) {
      std::cout << 0;

    } else {
      std::cout << -1;
    }
    return true;
  }
  return false;
}

int main() {
  auto input = ReadInput();
  if (SimplestCase(input)) {
    return 0;
  }
  std::vector<int> less_dp(input.max_floor + 1, 0);
  for (int i = 1; i <= input.max_floor; ++i) {
    less_dp[i] = i - 1;
  }
  if (input.amount_of_helicopters == 1) {
    std::cout << less_dp[input.max_floor];
    return 0;
  }
  std::vector<int> dp(input.max_floor + 1, 0);
  for (int helicopters = 2; helicopters <= input.amount_of_helicopters;
       ++helicopters) {
    for (int floor = 2; floor <= input.max_floor; ++floor) {
      dp[floor] = std::max(less_dp[(floor + (3 - floor % 3)) / 3],
                           dp[floor - ((floor + (3 - floor % 3)) / 3)]) +
                  1;
      dp[floor] = std::min(
          dp[floor], std::max(less_dp[(floor + (2 - floor % 2)) / 2],
                              dp[floor - ((floor + (2 - floor % 2)) / 2)]) +
                         1);
      dp[floor] = std::min(dp[floor], less_dp[floor - 1] + 1);
    }
    less_dp = dp;
  }
  std::cout << dp[input.max_floor];
}
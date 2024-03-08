#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

const int64_t kModule = 1'000'000'007;
const int64_t kBiggerModule = kModule * 2;

int64_t Mod(int64_t value, int64_t module) {
  int64_t answer = value % module;
  if (answer < 0) {
    answer = module + answer;
  }
  return answer;
}

int main() {
  int amount_of_numbers = 0;
  std::cin >> amount_of_numbers;
  std::vector<int64_t> numbers(amount_of_numbers);
  int64_t max_number = 0;
  for (int i = 0; i < amount_of_numbers; ++i) {
    std::cin >> numbers[i];
    max_number = std::max(max_number, numbers[i]);
  }
  std::vector<int64_t> dp(amount_of_numbers + 1, 0);
  std::vector<int64_t> to_minus(max_number + 1, 0);
  dp[1] = 1;
  to_minus[numbers[0]] = 1;
  for (int i = 2; i <= amount_of_numbers; ++i) {
    dp[i] = Mod((dp[i - 1] * 2 + 1 - to_minus[numbers[i - 1]]), kBiggerModule);
    to_minus[numbers[i - 1]] =
        Mod((to_minus[numbers[i - 1]] + dp[i] - dp[i - 1]), kBiggerModule);
  }
  std::cout << dp[amount_of_numbers] % kModule;
}
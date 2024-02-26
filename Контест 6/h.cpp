#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

int main() {
  std::string cj;
  std::cin >> cj;
  std::string caesar;
  std::cin >> caesar;

  std::vector<std::vector<int>> dp(cj.size() + 1,
                                   std::vector<int>(caesar.size() + 1, 0));
  for (size_t i = 1; i < dp.size(); ++i) {
    for (size_t j = 1; j < dp[i].size(); ++j) {
      if (cj[i - 1] == caesar[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }
  std::cout << dp[dp.size() - 1][dp[0].size() - 1] << "\n";
  std::stack<size_t> cj_answer;
  std::stack<size_t> caesar_answer;
  size_t cj_index = cj.size();
  size_t caesar_index = caesar.size();
  while (cj_index > 0 || caesar_index > 0) {
    if (cj_index == 0) {
      --caesar_index;
      continue;
    }
    if (caesar_index == 0) {
      --cj_index;
      continue;
    }
    if (cj[cj_index - 1] == caesar[caesar_index - 1]) {
      cj_answer.push(cj_index);
      caesar_answer.push(caesar_index);
      --cj_index;
      --caesar_index;
      continue;
    }
    if (dp[cj_index - 1][caesar_index] > dp[cj_index][caesar_index - 1]) {
      --cj_index;
      continue;
    }
    --caesar_index;
  }
  while (!cj_answer.empty()) {
    std::cout << cj_answer.top() << " ";
    cj_answer.pop();
  }
  std::cout << "\n";
  while (!caesar_answer.empty()) {
    std::cout << caesar_answer.top() << " ";
    caesar_answer.pop();
  }
}
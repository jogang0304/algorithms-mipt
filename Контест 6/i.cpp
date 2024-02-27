#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2e9;

struct Element {
  int value;
  int index_in_cj;
  int index_in_dj;
};

std::pair<int, int> Max(const std::pair<int, int>& first_pair,
                        const std::pair<int, int>& second_pair) {
  if (first_pair.first > second_pair.first) {
    return first_pair;
  }
  if (first_pair.first < second_pair.first) {
    return second_pair;
  }
  return {first_pair.first, std::min(first_pair.second, second_pair.second)};
}

int GetLengthOfMaxSubsequence(const std::vector<Element>& dp_length_element,
                              int current, int max_answer) {
  int left = 0;
  int right = max_answer;
  int middle;
  while (right - left > 1) {
    middle = (left + right) / 2;
    if (dp_length_element[middle].value < current) {
      left = middle;
    } else {
      right = middle;
    }
  }
  return right;
}

int main() {
  int cj_amount_of_moves;
  int dj_amount_of_moves;
  std::cin >> cj_amount_of_moves >> dj_amount_of_moves;
  std::vector<int> cj_moves(cj_amount_of_moves);
  std::vector<int> dj_moves(dj_amount_of_moves);
  for (int i = 0; i < cj_amount_of_moves; i++) {
    std::cin >> cj_moves[i];
  }
  for (int i = 0; i < dj_amount_of_moves; i++) {
    std::cin >> dj_moves[i];
  }
  if (dj_amount_of_moves < cj_amount_of_moves) {
    std::swap(cj_amount_of_moves, dj_amount_of_moves);
    std::swap(cj_moves, dj_moves);
  }
  std::vector<std::vector<std::pair<int, int>>> dp(
      cj_amount_of_moves + 1,
      std::vector<std::pair<int, int>>(
          dj_amount_of_moves + 1, {0, -1}));  // pair - max_length, last_element
  std::vector<Element> dp_length_element(cj_amount_of_moves + 1,
                                         {kInf, -1, -1});
  dp_length_element[0] = {-kInf, -1, -1};
  for (int i = 1; i <= cj_amount_of_moves; i++) {
    for (int j = 1; j <= dj_amount_of_moves; j++) {
      if (cj_moves[i - 1] == dj_moves[j - 1] &&
          cj_moves[i - 1] > dp[i - 1][j - 1].second) {
        dp[i][j] = {dp[i - 1][j - 1].first + 1, cj_moves[i - 1]};
      } else {
        dp[i][j] = Max(dp[i - 1][j], dp[i][j - 1]);
      }
      if (cj_moves[i - 1] == dj_moves[j - 1]) {
        int length_of_max_subsequence = GetLengthOfMaxSubsequence(
            dp_length_element, cj_moves[i - 1], std::min(i, j));
        if (length_of_max_subsequence >=
                static_cast<int>(dp_length_element.size()) ||
            dp_length_element[length_of_max_subsequence].value == kInf) {
          continue;
        }
        if (dp_length_element[length_of_max_subsequence - 1].value <
                cj_moves[i - 1] &&
            dp_length_element[length_of_max_subsequence - 1].index_in_cj <
                i - 1 &&
            dp_length_element[length_of_max_subsequence - 1].index_in_dj <
                j - 1 &&
            dp_length_element[length_of_max_subsequence].value >
                cj_moves[i - 1]) {
          dp_length_element[length_of_max_subsequence] = {cj_moves[i - 1],
                                                          i - 1, j - 1};
        }
      }
    }
  }
  int left = 0;
  int right = cj_amount_of_moves;
  int middle;
  while (right - left > 1) {
    middle = (left + right) / 2;
    if (dp_length_element[middle].value < kInf) {
      left = middle;
    } else {
      right = middle;
    }
  }
  std::cout << left << std::endl;
}
#include <algorithm>
#include <iostream>
#include <vector>

const int64_t kInf = 2'000'000'000;

void CountSums(std::vector<int64_t>& weights, std::vector<int64_t>& sums,
               int64_t points_for_second) {
  sums[0] = 0;
  for (int i = 0; i < points_for_second; ++i) {
    sums[0] += weights[i];
  }
  for (size_t i = 1; i < sums.size() - points_for_second; ++i) {
    sums[i] = sums[i - 1] + weights[i + points_for_second - 1] - weights[i - 1];
  }
}

int64_t Min(std::vector<int64_t>& array, int64_t left, int64_t right) {
  int64_t answer = kInf;
  for (int64_t i = left; i < right; ++i) {
    answer = std::min(answer, array[i]);
  }
  return answer;
}
int64_t Max(std::vector<int64_t>& array, int64_t left, int64_t right) {
  int64_t answer = -kInf;
  for (int64_t i = left; i < right; ++i) {
    answer = std::max(answer, array[i]);
  }
  return answer;
}

int64_t GetSecondResultByFirstIndex(int64_t first_answer_index,
                                    std::vector<int64_t>& sums,
                                    int64_t amount_of_points,
                                    int64_t points_for_second) {
  int64_t second_answer = Max(
      sums, first_answer_index + 1,
      std::min(first_answer_index + amount_of_points - points_for_second + 1,
               static_cast<int64_t>(sums.size())));
  if (first_answer_index + amount_of_points + 1 <
      static_cast<int64_t>(sums.size())) {
    second_answer = std::max(
        second_answer,
        Max(sums, first_answer_index + amount_of_points + 1, sums.size()));
  }
  if (first_answer_index >= points_for_second) {
    second_answer =
        std::max(second_answer,
                 Max(sums, 0, first_answer_index - points_for_second + 1));
  }
  return second_answer;
}

int main() {
  int64_t amount_of_points;
  std::cin >> amount_of_points;
  std::vector<int64_t> weights_cycle(amount_of_points * 2);
  for (int64_t i = 0; i < amount_of_points; ++i) {
    std::cin >> weights_cycle[i];
  }
  for (int64_t i = amount_of_points; i < amount_of_points * 2; ++i) {
    weights_cycle[i] = weights_cycle[i - amount_of_points];
  }
  int64_t double_sum = 0;
  for (auto element : weights_cycle) {
    double_sum += element;
  }
  int64_t points_for_second = amount_of_points / 2;

  // сумма на отрезке [i:i + points_for_second)
  std::vector<int64_t> sums(amount_of_points * 2 - points_for_second);
  CountSums(weights_cycle, sums, points_for_second);

  std::vector<int64_t> min_array(sums.size());
  std::vector<int64_t> max_array(sums.size());

  



  // какой счёт будет у первого игрока, если первым ходом он возьмёт вершину i
  std::vector<int64_t> first_result(amount_of_points);
  for (int64_t i = 0; i < amount_of_points; ++i) {
    int64_t second_result = GetSecondResultByFirstIndex(
        i, sums, amount_of_points, points_for_second);
    first_result[i] = (double_sum / 2) - second_result;
  }

  int64_t first_answer = 0;
  int64_t first_answer_index = 0;
  for (int64_t i = 0; i < amount_of_points; ++i) {
    if (first_result[i] > first_answer) {
      first_answer = first_result[i];
      first_answer_index = i;
    }
  }
  int64_t second_answer = GetSecondResultByFirstIndex(
      first_answer_index, sums, amount_of_points, points_for_second);
  std::cout << first_answer << " " << second_answer;
}
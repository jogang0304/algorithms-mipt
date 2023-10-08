#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

const int kOutputPrecision = 10;

std::vector<long double> GetPrefixSumsOfLogarithms(
    std::vector<long double>& values) {
  std::vector<long double> prefix_log_sum(values.size() + 1);
  prefix_log_sum[0] = 0;
  for (size_t i = 1; i < prefix_log_sum.size(); ++i) {
    prefix_log_sum[i] = prefix_log_sum[i - 1] + std::log2(values[i - 1]);
  }
  return prefix_log_sum;
}

long double GetAnswer(std::vector<long double>& prefix_log_sum, int left,
                      int right) {
  auto log_sum = prefix_log_sum[right + 1] - prefix_log_sum[left];
  auto power = log_sum / (right - left + 1);
  auto answer = std::pow(2, power);
  return answer;
}

int main() {
  size_t number_of_elements;
  std::cin >> number_of_elements;
  std::vector<long double> array_of_numbers(number_of_elements);
  for (auto& element : array_of_numbers) {
    std::cin >> element;
  }

  std::vector<long double> prefix_log_sum =
      GetPrefixSumsOfLogarithms(array_of_numbers);

  int number_of_questions;
  std::cin >> number_of_questions;
  std::cout << std::setprecision(kOutputPrecision) << std::fixed;
  int left;
  int right;
  for (int question = 0; question < number_of_questions; ++question) {
    std::cin >> left >> right;
    std::cout << GetAnswer(prefix_log_sum, left, right) << "\n";
  }
}
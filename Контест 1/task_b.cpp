#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
  size_t number_of_elements;
  std::cin >> number_of_elements;
  std::vector<long double> array_of_numbers(number_of_elements);
  for (auto& element : array_of_numbers) {
    std::cin >> element;
  }

  std::vector<long double> prefix_log_sum(array_of_numbers.size() + 1);
  prefix_log_sum[0] = 0;
  for (size_t i = 1; i < prefix_log_sum.size(); ++i) {
    prefix_log_sum[i] =
        prefix_log_sum[i - 1] + std::log2(array_of_numbers[i - 1]);
  }

  int number_of_questions;
  std::cin >> number_of_questions;
  const int kOutputPrecision = 10;
  std::cout << std::setprecision(kOutputPrecision) << std::fixed;
  int left;
  int right;
  for (int question = 0; question < number_of_questions; ++question) {
    std::cin >> left >> right;
    // geometric mean = root(e^{lna1 + lna2 + ...}, r-l+1) =
    // = e^{sum(ln a) / (r-l+1)}
    auto log_sum = prefix_log_sum[right + 1] - prefix_log_sum[left];
    auto power = log_sum / (right - left + 1);
    auto answer = std::pow(2, power);
    std::cout << answer << "\n";
  }
}
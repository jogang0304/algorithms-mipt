#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

struct Statistic {
  int value = 0;
  int index = 0;
};

class MinSparseTable {
 public:
  MinSparseTable(std::vector<int>& array) {
    log_.resize(array.size() + 1);
    log_[1] = 0;
    for (int i = 2; i <= static_cast<int>(array.size()); i++) {
      log_[i] = log_[i / 2] + 1;
    }
    table_.resize(log_[array.size()] + 1, std::vector<Statistic>(0));
    for (int i = 0; i < static_cast<int>(array.size()); i++) {
      table_[0].push_back({array[i], i});
    }
    for (int k = 1; k <= log_[array.size()]; k++) {
      for (int i = 0;
           i < static_cast<int>(table_[k - 1].size()) - (1 << (k - 1)); ++i) {
        if (table_[k - 1][i].value <= table_[k - 1][i + (1 << (k - 1))].value) {
          table_[k].push_back(table_[k - 1][i]);
        } else {
          table_[k].push_back(table_[k - 1][i + (1 << (k - 1))]);
        }
      }
    }
  }

  Statistic GetMin(int left, int right) {
    if (left > right) {
      return Statistic{kInf, -1};
    }
    int needed_range = log_[right - left + 1];
    if (table_[needed_range][left].value <=
        table_[needed_range][right - (1 << needed_range) + 1].value) {
      return table_[needed_range][left];
    }
    return table_[needed_range][right - (1 << needed_range) + 1];
  }

 private:
  std::vector<std::vector<Statistic>> table_;
  std::vector<int> log_;
};

class RMQ {
 public:
  RMQ(std::vector<int>& array) : table_(array) {}

  int GetSecondStatistic(int left, int right) {
    Statistic first_statistic = table_.GetMin(left, right);
    Statistic second_statistic_left =
        table_.GetMin(left, first_statistic.index - 1);
    Statistic second_statistic_right =
        table_.GetMin(first_statistic.index + 1, right);
    if (second_statistic_left.value < second_statistic_right.value) {
      return second_statistic_left.value;
    }
    return second_statistic_right.value;
  }

 private:
  MinSparseTable table_;
};

int main() {
  int amount_of_numbers;
  int amount_of_questions;
  std::cin >> amount_of_numbers >> amount_of_questions;
  std::vector<int> array(amount_of_numbers);
  for (auto& element : array) {
    std::cin >> element;
  }
  RMQ rmq(array);
  for (int i = 0; i < amount_of_questions; ++i) {
    int left;
    int right;
    std::cin >> left >> right;
    std::cout << rmq.GetSecondStatistic(left - 1, right - 1) << "\n";
  }
}
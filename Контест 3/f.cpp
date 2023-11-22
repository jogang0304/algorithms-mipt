#include <algorithm>
#include <iostream>
#include <vector>

inline int AndHelper(int number) { return (number & (number + 1)); }
inline int OrHelper(int number) { return (number | (number + 1)); }

class DoubleFenwick {
 public:
  DoubleFenwick(size_t height, size_t width)
      : sums_(height + 1, std::vector<int>(width + 1, 0)) {}
  void Add(int index, int compare_value, int value);
  int Get(int left_index, int right_index, int min_value, int max_value);

 private:
  std::vector<std::vector<int>> sums_;
};

void DoubleFenwick::Add(int index, int compare_value, int value) {
  for (int i = index; i < static_cast<int>(sums_.size()); i = OrHelper(i)) {
    for (int j = compare_value; j < static_cast<int>(sums_[i].size());
         j = OrHelper(j)) {
      sums_[i][j] += value;
    }
  }
}

int DoubleFenwick::Get(int left_index, int right_index, int min_value,
                       int max_value) {
  int prefix22 = 0;
  for (int i = right_index; i >= 0; i = AndHelper(i) - 1) {
    for (int j = max_value; j >= 0; j = AndHelper(j) - 1) {
      prefix22 += sums_[i][j];
    }
  }
  int prefix12 = 0;
  for (int i = left_index - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int j = max_value; j >= 0; j = AndHelper(j) - 1) {
      prefix12 += sums_[i][j];
    }
  }
  int prefix21 = 0;
  for (int i = right_index; i >= 0; i = AndHelper(i) - 1) {
    for (int j = min_value - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix21 += sums_[i][j];
    }
  }
  int prefix11 = 0;
  for (int i = left_index - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int j = min_value - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix11 += sums_[i][j];
    }
  }
  return prefix22 - prefix12 - prefix21 + prefix11;
}

int main() {
  int amount_of_numbers = 0;
  int amount_of_questions = 0;
  std::cin >> amount_of_numbers >> amount_of_questions;
  DoubleFenwick tree(amount_of_numbers, amount_of_numbers);
  for (int i = 0; i < amount_of_numbers; ++i) {
    int number;
    std::cin >> number;
    tree.Add(i + 1, number, 1);
  }
  for (int step = 0; step < amount_of_questions; ++step) {
    int left_index;
    int right_index;
    int min_value;
    int max_value;
    std::cin >> left_index >> right_index >> min_value >> max_value;
    std::cout << tree.Get(left_index, right_index, min_value, max_value)
              << "\n";
  }
}
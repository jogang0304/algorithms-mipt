#include <iostream>
#include <vector>

const int kNull = -1000000000;

inline int AndHelper(int number) { return number & (number + 1); }
inline int OrHelper(int number) { return number | (number + 1); }

class FenwickTree {
 public:
  FenwickTree(int size) : tree_(size) {}

  void Add(int index, int value) {
    for (int i = index; i < static_cast<int>(tree_.size()); i = OrHelper(i)) {
      tree_[i] += value;
    }
  }

  int GetSum(int left, int right) {
    int right_sum = 0;
    for (int i = right; i >= 0; i = AndHelper(i) - 1) {
      right_sum += tree_[i];
    }
    int left_sum = 0;
    for (int i = left - 1; i >= 0; i = AndHelper(i) - 1) {
      left_sum += tree_[i];
    }
    return right_sum - left_sum;
  }

 private:
  std::vector<int> tree_;
};

class Map {
 private:
  std::vector<int> array_;
  int size_;

 public:
  Map(int size) : array_(size, kNull), size_(0) {}

  int Get(int index) const {
    if (index < static_cast<int>(array_.size())) {
      return array_[index];
    }
    return kNull;
  }

  void Set(int index, int number) {
    if (index < static_cast<int>(array_.size())) {
      if (array_[index] == kNull) {
        ++size_;
      }
      array_[index] = number;
    }
  }

  int Size() const { return size_; }
};

int main() {
  int amount_of_hours;
  std::cin >> amount_of_hours;
  FenwickTree tree(amount_of_hours + 1);
  Map map(amount_of_hours + 1);
  for (int i = 0; i < amount_of_hours; ++i) {
    int current;
    std::cin >> current;
    tree.Add(i + 1, (i % 2 == 0) ? current : -current);
    map.Set(i + 1, current);
  }
  int amount_of_questions;
  std::cin >> amount_of_questions;
  for (int question = 0; question < amount_of_questions; ++question) {
    int command;
    int input_1;
    int input_2;
    std::cin >> command >> input_1 >> input_2;
    if (command == 0) {
      int current_value = map.Get(input_1);
      int to_add = input_2 - current_value;
      if (input_1 % 2 == 0) {
        to_add = -to_add;
      }
      tree.Add(input_1, to_add);
      map.Set(input_1, input_2);
    } else {
      int answer = tree.GetSum(input_1, input_2);
      if (input_1 % 2 == 0) {
        answer = -answer;
      }
      std::cout << answer << "\n";
    }
  }
}
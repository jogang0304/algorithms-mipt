#include <algorithm>
#include <iostream>
#include <vector>

const int kNull = -1;
const int kMaxUsers = 100000;
const int kMaxPage = 42195;

inline int AndHelper(int number) { return number & (number + 1); }
inline int OrHelper(int number) { return number | (number + 1); }

class FenwickTree {
 private:
  std::vector<int> array_;

 public:
  FenwickTree(int size) : array_(size, 0) {}

  void Add(int number) {
    for (int i = number; i < static_cast<int>(array_.size()); i = OrHelper(i)) {
      array_[i] += 1;
    }
  }

  void Decrease(int number) {
    for (int i = number; i < static_cast<int>(array_.size()); i = OrHelper(i)) {
      array_[i] -= 1;
    }
  }

  int GetSum(int left, int right) {
    int right_sum = 0;
    for (int i = right - 1; i >= 0; i = AndHelper(i) - 1) {
      right_sum += array_[i];
    }
    int left_sum = 0;
    for (int i = left - 1; i >= 0; i = AndHelper(i) - 1) {
      left_sum += array_[i];
    }
    return right_sum - left_sum;
  }
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
  int amount_of_questions;
  std::cin >> amount_of_questions;
  Map map(kMaxUsers + 1);
  FenwickTree tree(kMaxPage + 1);
  for (int question = 0; question < amount_of_questions; ++question) {
    std::string command;
    std::cin >> command;
    if (command == "CHEER") {
      int user;
      std::cin >> user;
      double answer = 0;
      int user_page = map.Get(user);
      if (user_page != kNull) {
        if (map.Size() == 1) {
          answer = 1;
        } else {
          int less_than_user = tree.GetSum(0, user_page);
          int all_users = map.Size();
          answer = static_cast<double>(less_than_user) / (all_users - 1);
        }
      }
      std::cout << answer << "\n";
    } else {
      int user;
      int page;
      std::cin >> user >> page;
      tree.Add(page);
      int user_page = map.Get(user);
      if (user_page != kNull) {
        tree.Decrease(user_page);
      }
      map.Set(user, page);
    }
  }
}
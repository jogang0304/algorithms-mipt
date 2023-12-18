#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

struct Matrix {
  int left = 0;
  int right = 0;
};

bool operator<(const Matrix& lhs, const Matrix& rhs) {
  return (lhs.left < rhs.left) ||
         (lhs.left == rhs.left && lhs.right > rhs.right);
}

int AndHelper(int number) { return number & (number + 1); }
int OrHelper(int number) { return number | (number + 1); }

class FenwickTree {
 public:
  FenwickTree(int size) : array_(size) {}

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

 private:
  std::vector<int> array_;
};

void Input(std::vector<Matrix>& array, std::map<Matrix, int>& similar_matrixes,
           std::set<int>& rights) {
  int amount_of_matrixes;
  std::cin >> amount_of_matrixes;
  int max_right = 0;
  for (int i = 0; i < amount_of_matrixes; ++i) {
    int left;
    int right;
    std::cin >> left >> right;
    max_right = std::max(max_right, right);
    array.push_back({left, right});
    similar_matrixes[{left, right}] += 1;
    rights.insert(right);
  }
}

int main() {
  std::vector<Matrix> array;
  std::map<Matrix, int> similar_matrixes;
  std::set<int> rights;
  Input(array, similar_matrixes, rights);
  std::map<int, int> right_to_number;
  int index = 0;
  for (auto value : rights) {
    right_to_number[value] = index++;
  }
  std::sort(array.begin(), array.end());
  FenwickTree tree(array.size() + 1);
  for (size_t i = 0; i < array.size(); ++i) {
    tree.Add(right_to_number[array[i].right]);
  }
  int64_t answer = 0;
  for (size_t i = 0; i < array.size(); ++i) {
    answer += tree.GetSum(0, right_to_number[array[i].right] + 1) -
              similar_matrixes[array[i]];
    similar_matrixes[array[i]] -= 1;
    tree.Decrease(right_to_number[array[i].right]);
  }
  std::cout << answer;
}
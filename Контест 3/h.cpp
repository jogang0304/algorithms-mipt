#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

inline int AndHelper(int number) { return number & (number + 1); }
inline int OrHelper(int number) { return number | (number + 1); }

struct Point {
  int x;
  int y;
  int weight;
};

struct ShrinkedValue {
  int real_index;
  int value;
};

class ShrinkedArray {
 public:
  ShrinkedArray(std::vector<int> array) : size_(array.size()) {
    std::sort(array.begin(), array.end());
    for (int i = 0; i < array.size(); ++i) {
      shrinked_values_.push_back({array[i], i});
    }
  }

  int ByShrinkedIndex(int shrinked_index) {
    return shrinked_values_[shrinked_index].value;
  }

  int ByRealIndex(int real_index) {
    auto it = std::lower_bound(shrinked_values_.begin(), shrinked_values_.end(),
                               real_index,
                               [](const ShrinkedValue& shrinked, int index) {
                                 return shrinked.real_index < index;
                               });
    return it->value;
  }

  int GetShrinkedIndex(int real_index) {
    auto it = std::lower_bound(shrinked_values_.begin(), shrinked_values_.end(),
                               real_index,
                               [](const ShrinkedValue& shrinked, int index) {
                                 return shrinked.real_index < index;
                               });
    return it - shrinked_values_.begin();
  }

  int Size() { return size_; }

 private:
  std::vector<ShrinkedValue> shrinked_values_;
  int size_;
};

class FenwickTree {
 public:
  FenwickTree(std::vector<Point>& array, std::vector<int>& ys)
      : tree_(array.size(), 0), array_(ys) {
    for (int i = 0; i < array.size(); ++i) {
      Increase(array[i].y, array[i].weight);
    }
  }
  void Increase(int index, int value) {
    int start_index = array_.GetShrinkedIndex(index);
    for (int i = start_index; i < tree_.size(); i = OrHelper(i)) {
      tree_[i] += value;
    }
  }

  int64_t GetPrefixSum(int right_index) {
    int shrinked_right_index = array_.GetShrinkedIndex(right_index);
    if (shrinked_right_index >= array_.Size() ||
        array_.ByShrinkedIndex(shrinked_right_index) > right_index) {
      shrinked_right_index--;
    }
    if (shrinked_right_index < 0) {
      return 0;
    }
    int64_t right_sum = 0;
    for (int i = right_index; i >= 0; i = AndHelper(i) - 1) {
      right_sum += tree_[i];
    }
    return right_sum;
  }

 private:
  std::vector<int> tree_;
  ShrinkedArray array_;
};

class FenwickFenwick {
 public:
  FenwickFenwick(std::vector<Point>& array) : tree_(array.size(), FenwickTree(0)) {}

 private:
  std::vector<FenwickTree> tree_;
}

int main() {
}
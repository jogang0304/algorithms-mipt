#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

class MaxTree {
 public:
  MaxTree(size_t size) : tree_(size * 4), array_size_(size) {}
  MaxTree(std::vector<int>& array) : MaxTree(array.size()) { Build(array); }

  void Set(size_t index, int value, size_t tree_index = 0, size_t tree_left = 0,
           size_t tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array_size_;
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index] = value;
      return;
    }
    size_t tree_middle = (tree_left + tree_right) / 2;
    if (index < tree_middle) {
      Set(index, value, tree_index * 2 + 1, tree_left, tree_middle);
    } else {
      Set(index, value, tree_index * 2 + 2, tree_middle, tree_right);
    }
    tree_[tree_index] =
        std::max(tree_[tree_index * 2 + 1], tree_[tree_index * 2 + 2]);
  }

  size_t Get(size_t index, int value, size_t tree_index = 0,
             size_t tree_left = 0, size_t tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array_size_;
    }
    if (tree_[tree_index] < value || index >= tree_right) {
      return kInf;
    }
    if (tree_left + 1 == tree_right) {
      return tree_left;
    }
    size_t tree_middle = (tree_left + tree_right) / 2;
    size_t answer = kInf;
    if (index < tree_middle) {
      if (tree_[tree_index * 2 + 1] >= value) {
        answer = Get(index, value, tree_index * 2 + 1, tree_left, tree_middle);
      }
    }
    if (answer < index || answer == kInf) {
      if (tree_[tree_index * 2 + 2] >= value) {
        answer = Get(index, value, tree_index * 2 + 2, tree_middle, tree_right);
      }
    }
    return answer;
  }

 private:
  std::vector<int> tree_;
  size_t array_size_;

  void Build(std::vector<int>& array, size_t tree_index = 0,
             size_t tree_left = 0, size_t tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array.size();
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index] = array[tree_left];
      return;
    }
    size_t tree_middle = (tree_left + tree_right) / 2;
    Build(array, tree_index * 2 + 1, tree_left, tree_middle);
    Build(array, tree_index * 2 + 2, tree_middle, tree_right);
    tree_[tree_index] =
        std::max(tree_[tree_index * 2 + 1], tree_[tree_index * 2 + 2]);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int amount_of_locations;
  int amount_of_questions;
  std::cin >> amount_of_locations >> amount_of_questions;
  std::vector<int> initial_array(amount_of_locations);
  for (int i = 0; i < amount_of_locations; ++i) {
    std::cin >> initial_array[i];
  }
  MaxTree tree(initial_array);
  for (int i = 0; i < amount_of_questions; ++i) {
    int command;
    int index;
    int value;
    std::cin >> command >> index >> value;
    if (command == 0) {
      tree.Set(index - 1, value);
    } else {
      size_t answer = tree.Get(index - 1, value);
      if (answer != kInf) {
        std::cout << answer + 1 << "\n";
      } else {
        std::cout << -1 << "\n";
      }
    }
  }
}

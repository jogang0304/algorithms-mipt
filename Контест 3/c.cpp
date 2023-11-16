#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

struct TreeData {
  size_t tree_index;
  size_t tree_left;
  size_t tree_right;
};

class SortTree {
 private:
  std::vector<std::vector<std::pair<int, int>>> tree_;
  int a_size_;
  static std::vector<std::pair<int, int>> Merge(
      std::vector<std::pair<int, int>>& left,
      std::vector<std::pair<int, int>>& right) {
    std::vector<std::pair<int, int>> answer;
    answer.reserve(left.size() + right.size());
    size_t left_index = 0;
    size_t right_index = 0;
    while (left_index < left.size() || right_index < right.size()) {
      if (left_index >= left.size()) {
        answer.push_back(right[right_index]);
        ++right_index;
      } else if (right_index >= right.size()) {
        answer.push_back(left[left_index]);
        ++left_index;
      } else if (left[left_index].first <= right[right_index].first) {
        answer.push_back(left[left_index]);
        ++left_index;
      } else {
        answer.push_back(right[right_index]);
        ++right_index;
      }
    }
    return answer;
  }

  void Build(std::vector<int>& array, size_t tree_index = 0,
             size_t tree_left = 0, size_t tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array.size();
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index].push_back(std::make_pair(array[tree_left], tree_left));
    } else {
      size_t tree_middle = (tree_left + tree_right) / 2;
      Build(array, tree_index * 2 + 1, tree_left, tree_middle);
      Build(array, tree_index * 2 + 2, tree_middle, tree_right);
      tree_[tree_index].clear();
      tree_[tree_index] =
          Merge(tree_[tree_index * 2 + 1], tree_[tree_index * 2 + 2]);
    }
  }

 public:
  SortTree(std::vector<int>& array)
      : tree_(array.size() * 4 + 4, std::vector<std::pair<int, int>>(0)),
        a_size_(array.size()) {
    Build(array);
  }

  void Update(size_t index, int value, size_t tree_index = 0,
              size_t tree_left = 0, size_t tree_right = 0) {
    if (tree_right == 0) {
      tree_right = a_size_;
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index].resize(1);
      tree_[tree_index][0] = std::make_pair(value, index);
    } else {
      size_t tree_middle = (tree_left + tree_right) / 2;
      if (index < tree_middle) {
        Update(index, value, tree_index * 2 + 1, tree_left, tree_middle);
      } else {
        Update(index, value, tree_index * 2 + 2, tree_middle, tree_right);
      }
      tree_[tree_index].clear();
      tree_[tree_index] =
          Merge(tree_[tree_index * 2 + 1], tree_[tree_index * 2 + 2]);
    }
  }
  int GetSubarray(size_t left, size_t right, int value,
                  TreeData tree_data = {0, 0, 0}) const {
    if (tree_data.tree_right == 0) {
      tree_data.tree_right = a_size_;
    }
    if (tree_[tree_data.tree_index][tree_[tree_data.tree_index].size() - 1]
            .first < value) {
      return -1;
    }
    int answer = -1;
    if (left == tree_data.tree_left && right == tree_data.tree_right) {
      if (left + 1 == right) {
        if (tree_[tree_data.tree_index][tree_[tree_data.tree_index].size() - 1]
                .first >= value) {
          answer = tree_[tree_data.tree_index]
                        [tree_[tree_data.tree_index].size() - 1]
                            .second;
        }
        return answer;
      }

      size_t tree_middle = (tree_data.tree_left + tree_data.tree_right) / 2;
      auto left_answer = GetSubarray(
          left, tree_middle, value,
          {tree_data.tree_index * 2 + 1, tree_data.tree_left, tree_middle});
      if (left_answer != -1) {
        return left_answer;
      }
      auto right_answer = GetSubarray(
          tree_middle, right, value,
          {tree_data.tree_index * 2 + 2, tree_middle, tree_data.tree_right});
      return right_answer;
    }
    size_t tree_middle = (tree_data.tree_left + tree_data.tree_right) / 2;
    std::pair<std::vector<std::pair<int, int>>, int> subtree;
    if (right <= tree_middle) {
      answer = GetSubarray(
          left, right, value,
          {tree_data.tree_index * 2 + 1, tree_data.tree_left, tree_middle});
    } else if (left >= tree_middle) {
      answer = GetSubarray(
          left, right, value,
          {tree_data.tree_index * 2 + 2, tree_middle, tree_data.tree_right});

    } else {
      auto left_answer = GetSubarray(
          left, tree_middle, value,
          {tree_data.tree_index * 2 + 1, tree_data.tree_left, tree_middle});
      if (left_answer != -1) {
        answer = left_answer;
      } else {
        auto right_answer = GetSubarray(
            tree_middle, right, value,
            {tree_data.tree_index * 2 + 2, tree_middle, tree_data.tree_right});
        answer = right_answer;
      }
    }
    return answer;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int amount_of_elements = 0;
  int amount_of_questions = 0;
  std::cin >> amount_of_elements >> amount_of_questions;
  std::vector<int> array(amount_of_elements + 1);
  for (int i = 1; i <= amount_of_elements; ++i) {
    std::cin >> array[i];
  }
  SortTree tree(array);
  for (int question = 0; question < amount_of_questions; ++question) {
    int command_number = 0;
    int index = 0;
    int value = 0;
    std::cin >> command_number >> index >> value;
    if (command_number == 0) {
      tree.Update(index, value);
    } else {
      auto answer = tree.GetSubarray(index, array.size(), value);
      std::cout << answer << "\n";
    }
  }
}
#include <algorithm>
#include <iostream>
#include <vector>

struct Node {
  std::vector<int> array;
};

struct Request {
  int left_index;
  int right_index;
  int min_value;
  int max_value;
};

class SortTree {
 public:
  SortTree(std::vector<int>& array)
      : tree_(array.size() * 4), array_size_(array.size()) {
    Build(array);
  }

  int Get(Request& request, int tree_index = 0, int tree_left = 0,
          int tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array_size_;
    }
    if (request.left_index >= tree_right || request.right_index <= tree_left) {
      return 0;
    }
    if (request.left_index <= tree_left && request.right_index >= tree_right) {
      int right_passing_index =
          std::upper_bound(tree_[tree_index].array.begin(),
                           tree_[tree_index].array.end(), request.max_value) -
          tree_[tree_index].array.begin();
      int left_passing_index =
          std::lower_bound(tree_[tree_index].array.begin(),
                           tree_[tree_index].array.end(), request.min_value) -
          tree_[tree_index].array.begin();
      return right_passing_index - left_passing_index;
    }
    int tree_middle = (tree_left + tree_right) / 2;
    return Get(request, tree_index * 2 + 1, tree_left, tree_middle) +
           Get(request, tree_index * 2 + 2, tree_middle, tree_right);
  }

 private:
  std::vector<Node> tree_;
  int array_size_;
  void Build(std::vector<int>& array, int tree_index = 0, int tree_left = 0,
             int tree_right = 0) {
    if (tree_right == 0) {
      tree_right = array_size_;
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index].array.push_back(array[tree_left]);
    } else {
      int tree_middle = (tree_left + tree_right) / 2;
      Build(array, tree_index * 2 + 1, tree_left, tree_middle);
      Build(array, tree_index * 2 + 2, tree_middle, tree_right);
      tree_[tree_index].array.resize(tree_[tree_index * 2 + 1].array.size() +
                                     tree_[tree_index * 2 + 2].array.size());
      std::merge(tree_[tree_index * 2 + 1].array.begin(),
                 tree_[tree_index * 2 + 1].array.end(),
                 tree_[tree_index * 2 + 2].array.begin(),
                 tree_[tree_index * 2 + 2].array.end(),
                 tree_[tree_index].array.begin());
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int amount_of_numbers;
  int amount_of_requests;
  std::cin >> amount_of_numbers >> amount_of_requests;
  std::vector<int> array(amount_of_numbers);
  for (auto& element : array) {
    std::cin >> element;
  }
  SortTree tree(array);
  for (int i = 0; i < amount_of_requests; ++i) {
    int left_index;
    int right_index;
    int min_value;
    int max_value;
    std::cin >> left_index >> right_index >> min_value >> max_value;
    Request request{left_index - 1, right_index, min_value, max_value};
    std::cout << tree.Get(request) << "\n";
  }
}

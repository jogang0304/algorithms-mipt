#include <iostream>
#include <memory>
#include <vector>

struct Node {
  int value = 0;
  int left = 0;
  int right = 0;
  std::shared_ptr<Node> left_child = nullptr;
  std::shared_ptr<Node> right_child = nullptr;

  Node(int value, int left, int right, std::shared_ptr<Node> left_child,
       std::shared_ptr<Node> right_child)
      : value(value),
        left(left),
        right(right),
        left_child(left_child),
        right_child(right_child) {}
};

class PersistentTree {
 public:
  PersistentTree(std::vector<int>& array)
      : roots_(0), array_size_(array.size()) {
    auto new_root = std::make_shared<Node>(0, 0, array_size_, nullptr, nullptr);
    roots_.push_back(new_root);
    Build(array, new_root);
  }

  void CreateNewVersion(int from_version, int index, int value) {
    roots_.push_back(
        std::make_shared<Node>(0, 0, array_size_, nullptr, nullptr));
    auto current_node = roots_.back();
    auto old_version_node = roots_[from_version];
    while (current_node->left + 1 != current_node->right) {
      int middle = (current_node->left + current_node->right) / 2;
      if (index < middle) {
        current_node->right_child = old_version_node->right_child;
        auto left_child = std::make_shared<Node>(0, current_node->left, middle,
                                                 nullptr, nullptr);
        current_node->left_child = left_child;
        current_node = current_node->left_child;
        old_version_node = old_version_node->left_child;
      } else {
        current_node->left_child = old_version_node->left_child;
        auto right_child = std::make_shared<Node>(
            0, middle, current_node->right, nullptr, nullptr);
        current_node->right_child = right_child;
        current_node = current_node->right_child;
        old_version_node = old_version_node->right_child;
      }
    }
    current_node->value = value;
  }

  int Get(int version, int index) {
    std::shared_ptr<Node> current_node = roots_[version];
    while (current_node->left + 1 != current_node->right) {
      int middle = (current_node->left + current_node->right) / 2;
      if (index < middle) {
        current_node = current_node->left_child;
      } else {
        current_node = current_node->right_child;
      }
    }
    return current_node->value;
  }

 private:
  std::vector<std::shared_ptr<Node>> roots_;
  int array_size_;

  void Build(std::vector<int>& array, std::shared_ptr<Node> node) {
    if (node->left + 1 == node->right) {
      node->value = array[node->left];
      return;
    }
    int middle = (node->left + node->right) / 2;
    node->left_child =
        std::make_shared<Node>(0, node->left, middle, nullptr, nullptr);
    node->right_child =
        std::make_shared<Node>(0, middle, node->right, nullptr, nullptr);
    Build(array, node->left_child);
    Build(array, node->right_child);
    node->value = node->left_child->value + node->right_child->value;
  }
};

int main() {
  int amount_of_elements;
  std::cin >> amount_of_elements;
  std::vector<int> array(amount_of_elements);
  for (auto& element : array) {
    std::cin >> element;
  }
  PersistentTree tree(array);
  int amount_of_questions;
  std::cin >> amount_of_questions;
  for (int i = 0; i < amount_of_questions; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "create") {
      int from_version;
      int index;
      int value;
      std::cin >> from_version >> index >> value;
      tree.CreateNewVersion(from_version - 1, index - 1, value);
    } else {
      int version;
      int index;
      std::cin >> version >> index;
      std::cout << tree.Get(version - 1, index - 1) << "\n";
    }
  }
}

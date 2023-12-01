#include <algorithm>
#include <iostream>
#include <vector>

const int64_t kInf = 1000000000000000000;
const int64_t kMod = 1'000'000'000;

// AVL tree (multiset), where all left children are less or equal and all right
// children are greater

class Node {
 public:
  int64_t value;
  int subtree_size = 1;
  int64_t subtree_sum = 0;
  int height = 1;
  int delta = 0;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
  Node* parent = nullptr;
  bool is_left_child = false;

  Node(int value) : value(value) {}

  ~Node() {
    delete left_child;
    delete right_child;
  }
};

class AVLTree {
 public:
  AVLTree() : root_(nullptr) {}

  void Insert(int value) {
    Node* to_insert = new Node(value);
    Node* current = root_;
    if (current == nullptr) {
      root_ = to_insert;
      Normalize(to_insert);
      return;
    }
    while (current != nullptr) {
      if (value <= current->value) {
        current->subtree_size++;
        if (current->left_child == nullptr) {
          to_insert->parent = current;
          to_insert->is_left_child = true;
          current->left_child = to_insert;
          break;
        }
        current = current->left_child;

      } else {
        if (current->right_child == nullptr) {
          to_insert->parent = current;
          to_insert->is_left_child = false;
          current->right_child = to_insert;
          break;
        }
        current = current->right_child;
      }
    }
    Normalize(to_insert);
  }

  int64_t GetElementGE(int64_t value) {
    Node* current = root_;
    int64_t best_passing_value = kInf;
    while (current != nullptr) {
      if (current->value >= value) {
        best_passing_value = std::min(best_passing_value, current->value);
      }
      if (value == current->value) {
        return value;
      }
      if (value < current->value) {
        if (current->left_child == nullptr) {
          return current->value;
        }
        current = current->left_child;
      } else {
        if (current->right_child == nullptr) {
          return best_passing_value != kInf ? best_passing_value : -1;
        }
        current = current->right_child;
      }
    }
    if (current == nullptr) {
      return -1;
    }
    return current->value;
  }

  static int GetHeight(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->height;
  }

  static int64_t GetSubtreeSum(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->subtree_sum;
  }

  static int GetSubtreeSize(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->subtree_size;
  }

  ~AVLTree() { delete root_; }

 private:
  Node* root_ = nullptr;

  static void RecalculateBasicInfo(Node* node) {
    node->height =
        std::max(GetHeight(node->left_child), GetHeight(node->right_child)) + 1;
    node->delta = GetHeight(node->left_child) - GetHeight(node->right_child);
    node->subtree_sum = GetSubtreeSum(node->left_child) +
                        GetSubtreeSum(node->right_child) + node->value;
    node->subtree_size = GetSubtreeSize(node->left_child) +
                         GetSubtreeSize(node->right_child) + 1;
  }

  void Normalize(Node* start_node) {
    Node* current = start_node;
    while (current != nullptr) {
      RecalculateBasicInfo(current);

      if (current->delta < -1) {
        SpinLeft(current);
        // current = current->parent;
      } else if (current->delta > 1) {
        SpinRight(current);
        // current = current->parent;
      }

      current = current->parent;
    }
  }

  void SpinRight(Node* node) {
    Node* left_child = node->left_child;
    Node* parent = node->parent;
    if (left_child->delta <= -1) {
      SpinLeft(left_child);
    } else if (left_child->delta >= 2) {
      SpinRight(left_child);
    }
    left_child = node->left_child;
    Node* left_right_child =
        (left_child != nullptr) ? left_child->right_child : nullptr;
    left_child->parent = parent;
    if (node->is_left_child) {
      if (parent != nullptr) {
        parent->left_child = left_child;
      }
      left_child->is_left_child = true;
    } else {
      if (parent != nullptr) {
        parent->right_child = left_child;
      }
      left_child->is_left_child = false;
    }
    left_child->right_child = node;
    node->parent = left_child;
    node->is_left_child = false;
    node->left_child = left_right_child;
    if (left_right_child != nullptr) {
      left_right_child->parent = node;
      left_right_child->is_left_child = true;
    }
    if (node == root_) {
      root_ = left_child;
    }
    RecalculateBasicInfo(node);
    RecalculateBasicInfo(left_child);
  }

  void SpinLeft(Node* node) {
    Node* right_child = node->right_child;
    Node* parent = node->parent;
    if (right_child->delta >= 1) {
      SpinRight(right_child);
    } else if (right_child->delta <= -2) {
      SpinLeft(right_child);
    }
    right_child = node->right_child;
    Node* right_left_child =
        (right_child != nullptr) ? right_child->left_child : nullptr;
    right_child->parent = parent;
    if (node->is_left_child) {
      if (parent != nullptr) {
        parent->left_child = right_child;
      }
      right_child->is_left_child = true;
    } else {
      if (parent != nullptr) {
        parent->right_child = right_child;
      }
      right_child->is_left_child = false;
    }
    right_child->left_child = node;
    node->parent = right_child;
    node->is_left_child = true;
    node->right_child = right_left_child;
    if (right_left_child != nullptr) {
      right_left_child->parent = node;
      right_left_child->is_left_child = false;
    }
    if (node == root_) {
      root_ = right_child;
    }
    RecalculateBasicInfo(node);
    RecalculateBasicInfo(right_child);
  }
};

int main() {
  int amount_of_questions;
  std::cin >> amount_of_questions;
  AVLTree tree;

  int64_t prev_next = 0;
  bool last_was_next = false;

  for (int i = 0; i < amount_of_questions; ++i) {
    std::string command;
    int64_t number;
    std::cin >> command >> number;
    if (command == "+") {
      if (last_was_next) {
        tree.Insert((number + prev_next) % kMod);
      } else {
        tree.Insert(number);
      }
      last_was_next = false;
    } else {
      auto answer = tree.GetElementGE(number);
      prev_next = answer;
      last_was_next = true;
      std::cout << answer << "\n";
    }
  }
}

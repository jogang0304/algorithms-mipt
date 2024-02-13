#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

const int kInfinityNumber = 1'000;
const std::string kInfinityWord(kInfinityNumber, 'z');
const std::pair<std::string, std::string> kInfinityPair = {kInfinityWord,
                                                           kInfinityWord};

// AVL tree (multiset), where all left children are less or equal and all right
// children are greater

template <typename T>
struct Node {
  T value;
  int subtree_size = 1;
  int height = 1;
  int delta = 0;
  Node<T>* left_child = nullptr;
  Node<T>* right_child = nullptr;
  Node<T>* parent = nullptr;
  bool is_left_child = false;

  Node(T value) : value(value) {}

  ~Node() {
    delete left_child;
    delete right_child;
  }
};

template <typename T>
class AVLTree {
 public:
  AVLTree(T infinity) : infinity_(infinity), root_(nullptr) {}

  void Insert(T value) {
    Node<T>* to_insert = new Node<T>(value);
    auto current = root_;
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

  T GetElementGE(T value) {
    auto current = root_;
    T best_passing_value = infinity_;
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
          return best_passing_value != infinity_ ? best_passing_value
                                                 : infinity_;
        }
        current = current->right_child;
      }
    }
    if (current == nullptr) {
      return infinity_;
    }
    return current->value;
  }

  static int GetHeight(Node<T>* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->height;
  }

  static int GetSubtreeSize(Node<T>* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->subtree_size;
  }

  ~AVLTree() { delete root_; }

 private:
  T infinity_;
  Node<T>* root_ = nullptr;

  static void RecalculateBasicInfo(Node<T>* node) {
    node->height =
        std::max(GetHeight(node->left_child), GetHeight(node->right_child)) + 1;
    node->delta = GetHeight(node->left_child) - GetHeight(node->right_child);
    node->subtree_size = GetSubtreeSize(node->left_child) +
                         GetSubtreeSize(node->right_child) + 1;
  }

  void Normalize(Node<T>* start_node) {
    auto current = start_node;
    while (current != nullptr) {
      RecalculateBasicInfo(current);

      if (current->delta < -1) {
        SpinLeft(current);
      } else if (current->delta > 1) {
        SpinRight(current);
      }

      current = current->parent;
    }
  }

  static void SwapWithLeftChild(Node<T>* node, Node<T>* left_child,
                                Node<T>* left_right_child) {
    left_child->right_child = node;
    node->parent = left_child;
    node->is_left_child = false;
    node->left_child = left_right_child;
    if (left_right_child != nullptr) {
      left_right_child->parent = node;
      left_right_child->is_left_child = true;
    }
  }

  void SpinRight(Node<T>* node) {
    auto left_child = node->left_child;
    auto parent = node->parent;
    if (left_child->delta <= -1) {
      SpinLeft(left_child);
    } else if (left_child->delta >= 2) {
      SpinRight(left_child);
    }
    left_child = node->left_child;
    auto left_right_child =
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
    SwapWithLeftChild(node, left_child, left_right_child);
    if (node == root_) {
      root_ = left_child;
    }
    RecalculateBasicInfo(node);
    RecalculateBasicInfo(left_child);
  }

  static void SwapWithRightChild(Node<T>* node, Node<T>* right_child,
                                 Node<T>* right_left_child) {
    right_child->left_child = node;
    node->parent = right_child;
    node->is_left_child = true;
    node->right_child = right_left_child;
    if (right_left_child != nullptr) {
      right_left_child->parent = node;
      right_left_child->is_left_child = false;
    }
  }

  void SpinLeft(Node<T>* node) {
    auto right_child = node->right_child;
    auto parent = node->parent;
    if (right_child->delta >= 1) {
      SpinRight(right_child);
    } else if (right_child->delta <= -2) {
      SpinLeft(right_child);
    }
    right_child = node->right_child;
    auto right_left_child =
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
    SwapWithRightChild(node, right_child, right_left_child);
    if (node == root_) {
      root_ = right_child;
    }
    RecalculateBasicInfo(node);
    RecalculateBasicInfo(right_child);
  }
};

bool operator<(const std::pair<std::string, std::string>& left,
               const std::pair<std::string, std::string>& right) {
  return (left.first < right.first) ||
         ((left.first == right.first) && (left.second < right.second));
}

int main() {
  int amount_of_pairs;
  std::cin >> amount_of_pairs;
  std::vector<std::pair<std::string, std::string>> pairs(amount_of_pairs);
  for (int i = 0; i < amount_of_pairs; ++i) {
    std::cin >> pairs[i].first >> pairs[i].second;
  }

  AVLTree<std::pair<std::string, std::string>> tree(kInfinityPair);
  for (size_t i = 0; i < pairs.size(); ++i) {
    tree.Insert(pairs[i]);
    tree.Insert({pairs[i].second, pairs[i].first});
  }
  int amount_of_questions;
  std::cin >> amount_of_questions;
  for (int i = 0; i < amount_of_questions; ++i) {
    std::string word;
    std::cin >> word;
    auto answer_pair = tree.GetElementGE({word, ""});
    std::cout << answer_pair.second << "\n";
  }
}
#include <iostream>
#include <memory>
#include <vector>

struct Node {
  std::shared_ptr<Node> left_child;
  std::shared_ptr<Node> right_child;
  std::shared_ptr<Node> parent;
  int value;
  bool is_red;
  bool is_null;
  Node()
      : left_child(nullptr),
        right_child(nullptr),
        parent(nullptr),
        value(-1),
        is_red(false),
        is_null(true) {}
};

class RedBlackTree {
 public:
  RedBlackTree() : root_(nullptr) {}

  bool Exists(int) const;
  void Insert(int);
  void Delete(int);
  int Next(int) const;
  int Prev(int) const;
  int Kth(int) const;

 private:
  std::shared_ptr<Node> root_;

  void RotateLeft(std::shared_ptr<Node>);
  void RotateRight(std::shared_ptr<Node>);
  void FixInsert(std::shared_ptr<Node>);
  void FixDelete(std::shared_ptr<Node>);

  std::shared_ptr<Node> GetParent(std::shared_ptr<Node> current) const;
  std::shared_ptr<Node> GetGrandparent(std::shared_ptr<Node> current) const;
  std::shared_ptr<Node> GetUncle(std::shared_ptr<Node> current) const;
};

std::shared_ptr<Node> RedBlackTree::GetParent(
    std::shared_ptr<Node> current) const {
  if (current == nullptr || current->parent == nullptr) {
    return nullptr;
  }
  return current->parent;
}

std::shared_ptr<Node> RedBlackTree::GetGrandparent(
    std::shared_ptr<Node> current) const {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  return current->parent->parent;
}

std::shared_ptr<Node> RedBlackTree::GetUncle(
    std::shared_ptr<Node> current) const {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  bool cur_parent_is_left_child =
      current->parent == current->parent->parent->left_child;
  if (cur_parent_is_left_child) {
    return current->parent->parent->right_child;
  } else {
    return current->parent->parent->left_child;
  }
}

// parent and grandparent exist
void RedBlackTree::RotateRight(std::shared_ptr<Node> current) {
  auto cur_left_child = current->left_child;
  auto cur_parent = current->parent;
  bool is_left_child = current == cur_parent->left_child;
  current->left_child = cur_left_child->right_child;
  current->parent = cur_left_child;
  cur_left_child->right_child = current;
  cur_left_child->parent = cur_parent;
  if (is_left_child) {
    cur_parent->left_child = cur_left_child;
  } else {
    cur_parent->right_child = cur_left_child;
  }
}

void RedBlackTree::RotateLeft(std::shared_ptr<Node> current) {
  auto cur_right_child = current->right_child;
  auto cur_parent = current->parent;
  bool is_left_child = current == cur_parent->left_child;
  current->right_child = cur_right_child->left_child;
  current->parent = cur_right_child;
  cur_right_child->left_child = current;
  cur_right_child->parent = cur_parent;
  if (is_left_child) {
    cur_parent->left_child = cur_right_child;
  } else {
    cur_parent->right_child = cur_right_child;
  }
}

void RedBlackTree::FixInsert(std::shared_ptr<Node> current) {
  auto cur_parent = GetParent(current);
  if (cur_parent == nullptr) {
    current->is_red = false;
    return;
  }
  if (!cur_parent->is_red) {
    return;
  }
  auto cur_uncle = GetUncle(current);
  auto cur_grandparent = GetGrandparent(current);
  if (cur_uncle->is_red) {
    cur_parent->is_red = false;
    cur_uncle->is_red = false;
    cur_grandparent->is_red = true;
    FixInsert(cur_grandparent);
    return;
  }
  if (current == cur_parent->left_child &&
      cur_parent == cur_grandparent->left_child) {
    cur_parent->is_red = false;
    cur_grandparent->is_red = true;
    RotateRight(cur_grandparent);
    return;
  }
  if (current == cur_parent->right_child &&
      cur_parent == cur_grandparent->right_child) {
    cur_parent->is_red = false;
    cur_grandparent->is_red = true;
    RotateLeft(cur_grandparent);
    return;
  }
  if (current == cur_parent->right_child &&
      cur_parent == cur_grandparent->left_child) {
    RotateLeft(cur_parent);
    FixInsert(cur_parent);
    return;
  }
  if (current == cur_parent->left_child &&
      cur_parent == cur_grandparent->right_child) {
    RotateRight(cur_parent);
    FixInsert(cur_parent);
    return;
  }
}

void RedBlackTree::FixDelete(std::shared_ptr<Node> current) {
  if (current->is_red && current->left_child->is_null &&
      current->right_child->is_null) {
    current = std::make_shared<Node>();
    return;
  }
  if (!current->is_red &&
      (current->left_child->is_null != current->right_child->is_null)) {
    if (current->right_child->is_null) {
      current = current->left_child;
    } else {
      current = current->right_child;
    }
    current->is_red = false;
  }
}

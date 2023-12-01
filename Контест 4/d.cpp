#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>

const int kInf = 2'000'000'000;

struct Node {
 public:
  std::shared_ptr<Node> left_child;
  std::shared_ptr<Node> right_child;
  std::shared_ptr<Node> parent;
  int value = -1;
  bool is_red = false;
  bool is_null = true;
  Node() : left_child(nullptr), right_child(nullptr), parent(nullptr) {}
};

class RedBlackTree {
 public:
  bool Exists(int) const;
  void Insert(int);
  void Delete(int);
  int Next(int) const;
  int Prev(int) const;
  int Kth(int) const;

  RedBlackTree() : root_(std::make_shared<Node>()) {
    root_->left_child = std::make_shared<Node>();
    root_->right_child = std::make_shared<Node>();
    root_->left_child->parent = root_;
    root_->right_child->parent = root_;
    root_->parent = std::make_shared<Node>();
  }

 private:
  std::shared_ptr<Node> root_;

  void RotateLeft(const std::shared_ptr<Node>&);
  void RotateRight(const std::shared_ptr<Node>&);
  void FixInsert(const std::shared_ptr<Node>&);
  void FixDelete(const std::shared_ptr<Node>&);

  std::shared_ptr<Node> GetParent(const std::shared_ptr<Node>& current) const;
  std::shared_ptr<Node> GetGrandparent(
      const std::shared_ptr<Node>& current) const;
  std::shared_ptr<Node> GetUncle(const std::shared_ptr<Node>& current) const;
  std::shared_ptr<Node> GetBrother(const std::shared_ptr<Node>& current) const;
};

std::shared_ptr<Node> RedBlackTree::GetParent(
    const std::shared_ptr<Node>& current) const {
  if (current == nullptr || current->parent == nullptr) {
    return nullptr;
  }
  return current->parent;
}

std::shared_ptr<Node> RedBlackTree::GetGrandparent(
    const std::shared_ptr<Node>& current) const {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  return current->parent->parent;
}

std::shared_ptr<Node> RedBlackTree::GetUncle(
    const std::shared_ptr<Node>& current) const {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  bool const cur_parent_is_left_child =
      current->parent == current->parent->parent->left_child;
  if (cur_parent_is_left_child) {
    return current->parent->parent->right_child;
  } else {
    return current->parent->parent->left_child;
  }
}

std::shared_ptr<Node> RedBlackTree::GetBrother(
    const std::shared_ptr<Node>& current) const {
  if (current == nullptr || current->parent == nullptr) {
    return nullptr;
  }
  bool const cur_is_left_child = current == current->parent->left_child;
  if (cur_is_left_child) {
    return current->parent->right_child;
  } else {
    return current->parent->left_child;
  }
}

bool RedBlackTree::Exists(int value) const {
  auto current = root_;
  while (!current->is_null) {
    if (value == current->value) {
      return true;
    }
    if (value < current->value) {
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  return false;
}

int RedBlackTree::Next(int value) const {
  if (root_->is_null) {
    return -1;
  }
  auto current = root_;
  int best_passing_value = kInf;
  while (!current->is_null) {
    if (value <= current->value) {
      best_passing_value = std::min(current->value, best_passing_value);
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  if (best_passing_value <= value || best_passing_value == kInf) {
    return -1;
  }
  return best_passing_value;
}

int RedBlackTree::Prev(int value) const {
  if (root_->is_null) {
    return -1;
  }
  auto current = root_;
  int best_passing_value = -kInf;
  while (!current->is_null) {
    if (value >= current->value) {
      best_passing_value = std::max(current->value, best_passing_value);
      current = current->right_child;
    } else {
      current = current->left_child;
    }
  }
  if (best_passing_value >= value || best_passing_value == -kInf) {
    return -1;
  }
  return best_passing_value;
}

// parent and grandparent exist
void RedBlackTree::RotateRight(const std::shared_ptr<Node>& current) {
  auto cur_left_child = current->left_child;
  auto cur_parent = current->parent;
  bool const is_left_child = current == cur_parent->left_child;
  current->left_child = cur_left_child->right_child;
  current->parent = cur_left_child;
  cur_left_child->right_child = current;
  cur_left_child->parent = cur_parent;
  if (is_left_child) {
    cur_parent->left_child = cur_left_child;
  } else {
    cur_parent->right_child = cur_left_child;
  }
  if (cur_parent->is_null) {
    root_ = cur_left_child;
  }
}

void RedBlackTree::RotateLeft(const std::shared_ptr<Node>& current) {
  auto cur_right_child = current->right_child;
  auto cur_parent = current->parent;
  bool const is_left_child = current == cur_parent->left_child;
  current->right_child = cur_right_child->left_child;
  current->parent = cur_right_child;
  cur_right_child->left_child = current;
  cur_right_child->parent = cur_parent;
  if (is_left_child) {
    cur_parent->left_child = cur_right_child;
  } else {
    cur_parent->right_child = cur_right_child;
  }
  if (cur_parent->is_null) {
    root_ = cur_right_child;
  }
}

void RedBlackTree::Insert(int value) {
  if (Exists(value)) {
    return;
  }
  auto current = root_;
  while (!current->is_null) {
    if (value < current->value) {
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  auto new_node = std::make_shared<Node>();
  new_node->value = value;
  new_node->is_red = true;
  new_node->is_null = false;
  new_node->left_child = std::make_shared<Node>();
  new_node->right_child = std::make_shared<Node>();
  new_node->left_child->parent = new_node;
  new_node->right_child->parent = new_node;
  if (current->parent == nullptr || current->parent->is_null) {
    root_->value = value;
    root_->is_null = false;
    root_->is_red = false;
    return;
  } else {
    if (value < current->parent->value) {
      current->parent->left_child = new_node;
    } else {
      current->parent->right_child = new_node;
    }
    new_node->parent = current->parent;
  }
  FixInsert(new_node);
}

void RedBlackTree::FixInsert(const std::shared_ptr<Node>& current) {
  auto cur_parent = GetParent(current);
  if (cur_parent == nullptr || cur_parent->is_null) {
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

void RedBlackTree::Delete(int value) {
  if (root_ == nullptr) {
    return;
  }
  if (!Exists(value)) {
    return;
  }
  auto current = root_;
  while (!current->is_null && current->value != value) {
    if (value < current->value) {
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  auto deleted_node = current;
  current = current->right_child;
  while (!current->is_null) {
    if (value < current->value) {
      current = current->left_child;
    } else if (value > current->value) {
      current = current->right_child;
    }
  }
  current = current->parent;
  std::swap(deleted_node->value, current->value);

  auto cur_parent = GetParent(current);

  auto not_null_child = (current->left_child->is_null) ? current->right_child
                                                       : current->left_child;
  if (current->is_red) {
    current = not_null_child;
    not_null_child->parent = current->parent;
    if (cur_parent->is_null) {
      root_ = not_null_child;
    }
    if (cur_parent->left_child == current) {
      cur_parent->left_child = not_null_child;
    } else {
      cur_parent->right_child = not_null_child;
    }
    return;
  }
  if (!current->is_red && not_null_child->is_red) {
    not_null_child->is_red = false;
    not_null_child->parent = current->parent;
    if (cur_parent->is_null) {
      root_ = not_null_child;
    }
    if (cur_parent->left_child == current) {
      cur_parent->left_child = not_null_child;
    } else {
      cur_parent->right_child = not_null_child;
    }
    return;
  }
  not_null_child->parent = cur_parent;
  if (cur_parent->is_null) {
    root_ = not_null_child;
  } else {
    if (current == cur_parent->left_child) {
      cur_parent->left_child = not_null_child;
    } else {
      cur_parent->right_child = not_null_child;
    }
  }
  FixDelete(not_null_child);
}

void RedBlackTree::FixDelete(const std::shared_ptr<Node>& current) {
  auto cur_parent = GetParent(current);
  auto cur_brother = GetBrother(current);
  if (cur_parent->is_null) {
    return;
  }
  if (cur_brother->is_red) {
    cur_brother->is_red = false;
    cur_parent->is_red = true;
    if (current == cur_parent->left_child) {
      RotateLeft(cur_parent);
    } else {
      RotateRight(cur_parent);
    }
  }
  auto cur_brother_left_child = cur_brother->left_child;
  auto cur_brother_right_child = cur_brother->right_child;
  if (!cur_parent->is_red && !cur_brother->is_red &&
      !cur_brother_left_child->is_red && !cur_brother_right_child->is_red) {
    cur_brother->is_red = true;
    FixDelete(cur_parent);
    return;
  }
  if (cur_parent->is_red && !cur_brother->is_red &&
      !cur_brother_left_child->is_red && !cur_brother_right_child->is_red) {
    cur_brother->is_red = true;
    cur_parent->is_red = false;
    return;
  }

  if (current == cur_parent->left_child && !cur_brother->is_red &&
      cur_brother_left_child->is_red && !cur_brother_right_child->is_red) {
    cur_brother->is_red = true;
    cur_brother_left_child->is_red = false;
    RotateRight(cur_brother);
  }
  if (current == cur_parent->right_child && !cur_brother->is_red &&
      !cur_brother_left_child->is_red && cur_brother_right_child->is_red) {
    cur_brother->is_red = true;
    cur_brother_right_child->is_red = false;
    RotateLeft(cur_brother);
  }
  std::swap(cur_brother->is_red, cur_parent->is_red);
  if (current == cur_parent->left_child && !cur_brother->is_red) {
    cur_brother_right_child->is_red = false;
    RotateLeft(cur_parent);
  } else {
    cur_brother_left_child->is_red = false;
    RotateRight(cur_parent);
  }
}

int main() {
  RedBlackTree tree;
  std::string command;
  while (std::cin >> command) {
    int value;
    std::cin >> value;
    if (command == "insert") {
      tree.Insert(value);
    }
    if (command == "delete") {
      tree.Delete(value);
    }
    if (command == "exists") {
      std::cout << (tree.Exists(value) ? "true" : "false") << "\n";
    }
    if (command == "next") {
      int next = tree.Next(value);
      if (next == value || next == -1) {
        std::cout << "none\n";
      } else {
        std::cout << next << "\n";
      }
    }
    if (command == "prev") {
      int prev = tree.Prev(value);
      if (prev == value || prev == -1) {
        std::cout << "none\n";
      } else {
        std::cout << prev << "\n";
      }
    }
  }
}

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

const int kInf = 2'000'000'000;

struct Node {
 public:
  Node* left_child = nullptr;
  Node* right_child = nullptr;
  Node* parent = nullptr;
  int value = -1;
  bool is_red = false;
  bool is_null = true;
  int count_elements_on_left = 0;
  Node() {}
  ~Node() {
    if (left_child != nullptr) {
      left_child->parent = nullptr;
      delete left_child;
      left_child = nullptr;
    }
    if (right_child != nullptr) {
      right_child->parent = nullptr;
      delete right_child;
      right_child = nullptr;
    }
  }
};

void DeleteNodeParent(Node* node) {
  if (node->parent->left_child == node) {
    node->parent->left_child = nullptr;
  } else if (node->parent->right_child == node) {
    node->parent->right_child = nullptr;
  }
  delete node->parent;
}

class RedBlackTree {
 public:
  bool Exists(int value) const;
  void Insert(int value);
  void Delete(int value);
  int Next(int value) const;
  int Prev(int value) const;
  int Kth(int statistic) const;

  RedBlackTree() : root_(new Node()) {
    delete root_->left_child;
    root_->left_child = new Node();
    delete root_->right_child;
    root_->right_child = new Node();
    delete root_->left_child->parent;
    root_->left_child->parent = root_;
    delete root_->right_child->parent;
    root_->right_child->parent = root_;
    delete root_->parent;
    root_->parent = new Node();
  }

  ~RedBlackTree() {
    if (root_->parent->left_child == root_) {
      root_->parent->left_child = nullptr;
    } else if (root_->parent->right_child == root_) {
      root_->parent->right_child = nullptr;
    }
    delete root_->parent;
    delete root_;
  }

 private:
  Node* root_;

  void RotateLeft(Node* current);
  void RotateRight(Node* current);
  void FixInsert(Node* current);
  void FixDelete(Node* current);

  static Node* GetParent(Node* current);
  static Node* GetGrandparent(Node* current);
  static Node* GetUncle(Node* current);
  static Node* GetBrother(Node* current);
};

Node* RedBlackTree::GetParent(Node* current) {
  if (current == nullptr || current->parent == nullptr) {
    return nullptr;
  }
  return current->parent;
}

Node* RedBlackTree::GetGrandparent(Node* current) {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  return current->parent->parent;
}

Node* RedBlackTree::GetUncle(Node* current) {
  if (current == nullptr || current->parent == nullptr ||
      current->parent->parent == nullptr) {
    return nullptr;
  }
  bool cur_parent_is_left_child =
      current->parent == current->parent->parent->left_child;
  if (cur_parent_is_left_child) {
    return current->parent->parent->right_child;
  }
  return current->parent->parent->left_child;
}
Node* RedBlackTree::GetBrother(Node* current) {
  if (current == nullptr || current->parent == nullptr) {
    return nullptr;
  }
  bool cur_is_left_child = current == current->parent->left_child;
  if (cur_is_left_child) {
    return current->parent->right_child;
  }
  return current->parent->left_child;
}

bool RedBlackTree::Exists(int value) const {
  Node* current = root_;
  while (current != nullptr && !current->is_null) {
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
  if (root_ == nullptr || root_->is_null) {
    return -kInf;
  }
  Node* current = root_;
  int best_passing_value = kInf;
  while (current != nullptr && !current->is_null) {
    if (value <= current->value) {
      best_passing_value = std::min(current->value, best_passing_value);
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  if (best_passing_value <= value || best_passing_value == kInf) {
    return -kInf;
  }
  return best_passing_value;
}

int RedBlackTree::Prev(int value) const {
  if (root_ == nullptr || root_->is_null) {
    return -kInf;
  }
  Node* current = root_;
  int best_passing_value = -kInf;
  while (current != nullptr && !current->is_null) {
    if (value >= current->value) {
      best_passing_value = std::max(current->value, best_passing_value);
      current = current->right_child;
    } else {
      current = current->left_child;
    }
  }
  if (best_passing_value >= value || best_passing_value == -kInf) {
    return -kInf;
  }
  return best_passing_value;
}

// parent and grandparent exist
void RedBlackTree::RotateRight(Node* current) {
  Node* cur_left_child = current->left_child;
  Node* cur_parent = current->parent;
  bool is_left_child = current == cur_parent->left_child;
  current->left_child = cur_left_child->right_child;
  current->count_elements_on_left--;
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

void RedBlackTree::RotateLeft(Node* current) {
  Node* cur_right_child = current->right_child;
  Node* cur_parent = current->parent;
  bool is_left_child = current == cur_parent->left_child;
  current->right_child = cur_right_child->left_child;
  current->parent = cur_right_child;
  cur_right_child->left_child = current;
  cur_right_child->count_elements_on_left++;
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

Node* CreateNewNode(int value) {
  Node* new_node = new Node();
  new_node->value = value;
  new_node->is_red = true;
  new_node->is_null = false;
  delete new_node->left_child;
  new_node->left_child = new Node();
  delete new_node->right_child;
  new_node->right_child = new Node();
  delete new_node->left_child->parent;
  new_node->left_child->parent = new_node;
  delete new_node->right_child->parent;
  new_node->right_child->parent = new_node;
  delete new_node->parent;
  new_node->parent = new Node();
  return new_node;
}

void RedBlackTree::Insert(int value) {
  if (Exists(value)) {
    return;
  }
  Node* current = root_;
  while (!current->is_null) {
    if (value < current->value) {
      current = current->left_child;
    } else {
      current = current->right_child;
    }
  }
  Node* new_node = CreateNewNode(value);
  if (current->parent == nullptr || current->parent->is_null) {
    delete root_->parent;
    delete root_;
    new_node->is_red = false;
    root_ = new_node;
    return;
  }
  if (value < current->parent->value) {
    if (!current->parent->left_child->is_null) {
      Node* existing_node = current->parent->left_child;
      current->parent->left_child = nullptr;
      delete existing_node;
    }
    current->parent->left_child = new_node;
    current->parent->count_elements_on_left++;
  } else {
    if (!current->parent->right_child->is_null) {
      Node* existing_node = current->parent->right_child;
      current->parent->right_child = nullptr;
      delete existing_node;
    }
    current->parent->right_child = new_node;
  }
  delete new_node->parent;
  new_node->parent = current->parent;
  delete current;
  FixInsert(new_node);
}

void RedBlackTree::FixInsert(Node* current) {
  Node* cur_parent = GetParent(current);
  if (cur_parent == nullptr || cur_parent->is_null) {
    current->is_red = false;
    return;
  }
  if (!cur_parent->is_red) {
    return;
  }
  Node* cur_uncle = GetUncle(current);
  Node* cur_grandparent = GetGrandparent(current);
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

void FindNode(Node*& current, int value) {
  while (!current->is_null && current->value != value) {
    if (value < current->value) {
      current = current->left_child;
    } else if (value > current->value) {
      current = current->right_child;
    }
  }
}

bool SimpleDeleteCases(Node*& current, Node*& cur_parent, Node*& not_null_child,
                       Node*& root) {
  if (current->is_red) {
    // current = not_null_child;
    not_null_child->parent = cur_parent;
    if (cur_parent->is_null) {
      root = not_null_child;
    }
    if (cur_parent->left_child == current) {
      cur_parent->left_child = not_null_child;
    } else {
      cur_parent->right_child = not_null_child;
    }
    return true;
  }
  if (!current->is_red && not_null_child->is_red) {
    not_null_child->is_red = false;
    not_null_child->parent = cur_parent;
    if (cur_parent->is_null) {
      root = not_null_child;
    }
    if (cur_parent->left_child == current) {
      cur_parent->left_child = not_null_child;
    } else {
      cur_parent->right_child = not_null_child;
    }
    return true;
  }
  return false;
}

void HandleRootDelete(Node* current, Node* cur_parent, Node*& root) {
  if (current->parent != nullptr && !current->parent->is_null &&
      current != cur_parent->left_child && current != cur_parent->right_child) {
    DeleteNodeParent(current);
    delete current;
    if (current == cur_parent->left_child) {
      cur_parent->left_child = new Node();
    } else {
      cur_parent->right_child = new Node();
    }
  } else if (current->parent == nullptr || current->parent->is_null) {
    DeleteNodeParent(current);
    delete current;
    root = new Node();
    root->is_red = false;
  }
}

void RedBlackTree::Delete(int value) {
  if (root_ == nullptr) {
    return;
  }
  if (!Exists(value)) {
    return;
  }
  Node* current = root_;
  FindNode(current, value);
  Node* deleted_node = current;
  current = current->right_child;
  FindNode(current, value);

  current = current->parent;
  std::swap(deleted_node->value, current->value);

  Node* cur_parent = GetParent(current);

  Node* not_null_child = (current->left_child->is_null) ? current->right_child
                                                        : current->left_child;
  bool simple_result =
      SimpleDeleteCases(current, cur_parent, not_null_child, root_);
  if (simple_result) {
    current->is_null = true;
    HandleRootDelete(current, cur_parent, root_);
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
  current->is_null = true;
  HandleRootDelete(current, cur_parent, root_);
}

void RedBlackTree::FixDelete(Node* current) {
  Node* cur_parent = GetParent(current);
  Node* cur_brother = GetBrother(current);
  if (cur_parent == nullptr || cur_parent->is_null) {
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
  Node* cur_brother_left_child = cur_brother->left_child;
  Node* cur_brother_right_child = cur_brother->right_child;
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

int RedBlackTree::Kth(int statistic) const {
  if (root_->is_null) {
    return -kInf;
  }
  Node* current = root_;
  while (!current->is_null) {
    if (statistic == current->count_elements_on_left) {
      return current->value;
    }
    if (statistic < current->count_elements_on_left) {
      current = current->left_child;
    } else {
      statistic -= current->count_elements_on_left + 1;
      current = current->right_child;
    }
  }
  return -kInf;
}

void HandleExists(RedBlackTree& tree, int value) {
  if (tree.Exists(value)) {
    std::cout << "true\n";
    return;
  }
  std::cout << "false\n";
}

void HandleNext(RedBlackTree& tree, int value) {
  int next = tree.Next(value);
  if (next != -kInf && next != value) {
    std::cout << next << "\n";
    return;
  }
  std::cout << "none\n";
}

void HandlePrev(RedBlackTree& tree, int value) {
  int prev = tree.Prev(value);
  if (prev != -kInf && prev != value) {
    std::cout << prev << "\n";
    return;
  }
  std::cout << "none\n";
}

void HandleKth(RedBlackTree& tree, int statistic) {
  int result = tree.Kth(statistic);
  if (result != -kInf) {
    std::cout << result << "\n";
    return;
  }
  std::cout << "none\n";
}

int main() {
  RedBlackTree tree;
  std::string command;
  while (std::cin >> command) {
    int value = 0;
    std::cin >> value;
    if (command == "insert") {
      tree.Insert(value);
    }
    if (command == "delete") {
      tree.Delete(value);
    }
    if (command == "exists") {
      HandleExists(tree, value);
    }
    if (command == "next") {
      HandleNext(tree, value);
    }
    if (command == "prev") {
      HandlePrev(tree, value);
    }
    if (command == "kth") {
      HandleKth(tree, value);
    }
  }
}

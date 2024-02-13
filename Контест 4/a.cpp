#include <algorithm>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

struct Node {
  Node* left_child = nullptr;
  Node* right_child = nullptr;
  int key = 0;
  int priority = 0;
  Node(int key = 0, int priority = 0) : key(key), priority(priority) {}
  ~Node() {
    delete left_child;
    delete right_child;
  }
};

class Treap {
 public:
  Treap() : root_(new Node()) {}
  Treap(const std::vector<std::pair<int, int>>& vec)
      : root_(new Node(vec[0].first, vec[0].second)) {
    std::stack<Node*> right_path;
    right_path.push(root_);
    for (size_t i = 1; i < vec.size(); ++i) {
      Node* last_popped_node = nullptr;
      while (!right_path.empty() &&
             vec[i].second < right_path.top()->priority) {
        last_popped_node = right_path.top();
        right_path.pop();
      }
      Node* new_node = new Node(vec[i].first, vec[i].second);
      if (right_path.empty()) {
        root_ = new_node;
      } else {
        right_path.top()->right_child = new_node;
      }
      new_node->left_child = last_popped_node;
      right_path.push(new_node);
    }
  }

  void Print() { PrintNode(root_, nullptr); }

  ~Treap() { delete root_; }

 private:
  Node* root_;

  static void PrintNode(Node* node, Node* parent) {
    if (node->left_child != nullptr) {
      PrintNode(node->left_child, node);
    }

    if (parent != nullptr) {
      std::cout << parent->key << " ";
    } else {
      std::cout << 0 << " ";
    }
    if (node->left_child == nullptr) {
      std::cout << 0 << " ";
    } else {
      std::cout << node->left_child->key << " ";
    }
    if (node->right_child == nullptr) {
      std::cout << 0 << " ";
    } else {
      std::cout << node->right_child->key << " ";
    }
    std::cout << "\n";

    if (node->right_child != nullptr) {
      PrintNode(node->right_child, node);
    }
  }
};

int main() {
  int amount_of_cars;
  std::cin >> amount_of_cars;
  std::vector<std::pair<int, int>> cars(amount_of_cars);
  for (int i = 0; i < amount_of_cars; ++i) {
    int useless_number;
    std::cin >> useless_number >> cars[i].second;
    cars[i].first = i + 1;
  }
  Treap tree(cars);
  std::cout << "YES\n";
  tree.Print();
}

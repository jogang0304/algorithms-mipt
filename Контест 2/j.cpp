#include <iostream>
#include <memory>
#include <vector>

class Node {
 private:
  int value;
  Node* parent;
  std::vector<std::shared_ptr<Node>> children;

 public:
  Node(int value = 0, Node* parent = nullptr) : value(value), parent(parent) {}
  int GetValue() const { return value; }
  void AddChild(std::shared_ptr<Node>& child) {
    children.push_back(child);
    child->parent = this;
  }
  void Merge(std::shared_ptr<Node>& other);
};

class BinomialHeap {
 private:
  std::vector<std::shared_ptr<Node>> trees;
  void Merge(BinomialHeap& other);

 public:
  BinomialHeap() : trees(1, nullptr) {}
  BinomialHeap(int value) : trees(1, std::make_shared<Node>(value)) {}
  BinomialHeap(const std::vector<std::shared_ptr<Node>>& trees)
      : trees(trees){};
};

void Node::Merge(std::shared_ptr<Node>& other) {
  this->AddChild(other);
  if (this->GetValue() < other->GetValue()) {
    return;
  }
  other = this;
}

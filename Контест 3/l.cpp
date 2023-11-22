#include <iostream>
#include <vector>

struct Node {
  int value;
  Node* left = nullptr;
  Node* right = nullptr;
};

class PersistentTree {
  public:
  PersistentTree(std::vector<int>& array) {
    roots_.push_back(nullptr);
    
  }
  private:
  std::vector<Node*> roots_;
};

// надо делать массив через ДО


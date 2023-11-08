#include <algorithm>
#include <iostream>
#include <vector>

const int kMaxPeople = 100000;

class SortTree {
 public:
  SortTree(int size = kMaxPeople);
  void Update(int person, int value, int tree_index, int tree_left,
              int tree_right);
  int CountLessThan(int value);

  double Cheer(int person);

 private:
  std::vector<std::vector<int>> nodes_;
  std::vector<int> people;

  std::vector<int> Merge_(int first_node, int second_node);
};

SortTree::SortTree(int size) {
  nodes_.resize(size * 4, std::vector<int>(0));
  people.resize(size, 0);
}

std::vector<int> SortTree::Merge_(int first_node, int second_node) {
  std::vector<int> result(0);
  int first_index = 0;
  int second_index = 0;
  while (first_index < nodes_[first_node].size() ||
         second_index < nodes_[second_node].size()) {
    if (first_index < nodes_[first_node].size() &&
        second_index < nodes_[second_node].size()) {
      if (nodes_[first_node][first_index] <=
          nodes_[second_node][second_index]) {
        result.push_back(nodes_[first_node][first_index++]);
      } else {
        result.push_back(nodes_[second_node][second_index++]);
      }
    } else if (first_index < nodes_[first_node].size()) {
      result.push_back(nodes_[first_node][first_index++]);
    } else {
      result.push_back(nodes_[second_node][second_index++]);
    }
  }
  return result;
}

void SortTree::Update(int person, int value, int tree_index = 0,
                      int tree_left = 0, int tree_right = -1) {
  if (tree_right == -1) {
    tree_right = people.size();
  }
  if (tree_left >= tree_right) {
    return;
  }
  if (tree_left + 1 == tree_right) {
    people[person] = value;
    nodes_[tree_index] = {value};
    return;
  }
  int tree_middle = (tree_right + tree_left) / 2;
  if (person < tree_middle) {
    Update(person, value, tree_index * 2 + 1, tree_left, tree_middle);
  } else {
    Update(person, value, tree_index * 2 + 2, tree_middle, tree_right);
  }
  nodes_[tree_index] = Merge_(tree_index * 2 + 1, tree_index * 2 + 2);
}

int SortTree::CountLessThan(int value) {
  int index_of_first_ge_1 = 0;
  int left = 0;
  int right = nodes_[0].size();
  while (right - left > 1) {
    int middle = (left + right) / 2;
    if (nodes_[0][middle] < 1) {
      left = middle;
    } else {
      right = middle;
    }
  }
  index_of_first_ge_1 = right;
  int index_of_first_ge_x = 0;
  left = 0;
  right = nodes_[0].size();
  while (right - left > 1) {
    int middle = (left + right) / 2;
    if (nodes_[0][middle] >= value) {
      right = middle;
    } else {
      left = middle;
    }
  }
  index_of_first_ge_x = right;
  int answer = index_of_first_ge_x - index_of_first_ge_1;
  return answer;
}

double SortTree::Cheer(int person) {
  int passing_people = CountLessThan(people[person]);
  double all_people = static_cast<double>(people.size());
  double answer = passing_people / all_people;
  return answer;
}

int main() {
  int amount_of_questions;
  std::cin >> amount_of_questions;
  SortTree tree;
  for (int step = 0; step < amount_of_questions; step++) {
    std::string command;
    std::cin >> command;
    if (command == "CHEER") {
      int person;
      std::cin >> person;
      std::cout << tree.Cheer(person) << std::endl;
    } else {
      int person;
      int value;
      std::cin >> person >> value;
      tree.Update(person, value);
    }
  }
}

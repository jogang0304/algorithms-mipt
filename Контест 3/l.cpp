#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

struct Change {
  size_t version;
  size_t index;
  int value;
  size_t from_version;
};

bool operator<(const Change& left, const Change& right) {
  return (left.version < right.version ||
          (left.version == right.version && left.index < right.index));
}

class PersistentArray {
 public:
  PersistentArray(std::vector<int>& array) {
    for (size_t i = 0; i < array.size(); ++i) {
      changes_.push_back({1, i + 1, array[i], 1});
    }
  }

  void Create(size_t version, size_t index, int value) {
    max_version_++;
    changes_.push_back({max_version_, index, value, version});
  }

  int Get(size_t version, size_t index) {
    auto it = std::lower_bound(changes_.begin(), changes_.end(),
                               Change{version, index, 0, 0});
    if (it == changes_.end() || it->version > version) {
      it = std::lower_bound(changes_.begin(), changes_.end(),
                            Change{version, 0, 0, 0});
    }
    while (it != changes_.begin() && it->index != index) {
      version = it->from_version;
      it = std::lower_bound(changes_.begin(), changes_.end(),
                            Change{version, index, 0, 0});
      if (it->version > version) {
        it = std::lower_bound(changes_.begin(), changes_.end(),
                              Change{version, 0, 0, 0});
      }
    }

    return it->value;
  }

 private:
  size_t max_version_ = 1;
  std::vector<Change> changes_;
};

int main() {
  // std::ios_base::sync_with_stdio(false);
  // std::cin.tie(NULL);
  // std::cout.tie(NULL);
  int amount_of_elements;
  std::cin >> amount_of_elements;
  std::vector<int> values(amount_of_elements);
  for (auto& element : values) {
    std::cin >> element;
  }
  PersistentArray array(values);
  int amount_of_questions;
  std::cin >> amount_of_questions;
  for (int i = 0; i < amount_of_questions; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "create") {
      size_t version;
      size_t index;
      int value;
      std::cin >> version >> index >> value;
      array.Create(version, index, value);
    } else {
      size_t version;
      size_t index;
      std::cin >> version >> index;
      std::cout << array.Get(version, index) << "\n";
    }
  }
}
#include <limits.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

const int kDefaultTableSize = 1'000;

size_t Hash(std::string str) {
  std::vector<int> map_char_to_number(CHAR_MAX + 1, 0);
  int cur_index = 1;
  for (char& letter : str) {
    if (map_char_to_number[letter] == 0) {
      map_char_to_number[letter] = cur_index++;
    }
    letter = map_char_to_number[letter] + 'a';
  }
  return std::hash<std::string>{}(str);
}

template <typename T>
bool operator<(const std::pair<T, int>& left, const std::pair<T, int>& right) {
  return left.first < right.first;
}

template <typename T>
class Bucket {
 public:
  Bucket() {}

  void Insert(std::pair<T, int> value);
  void Remove(std::pair<T, int> value);
  bool Has(T value) const;
  std::pair<T, int> Get(T value) const;

  ~Bucket() = default;

 private:
  std::list<std::pair<T, int>> elements_;
};

template <typename T>
void Bucket<T>::Insert(std::pair<T, int> value) {
  elements_.push_front(value);
}

template <typename T>
void Bucket<T>::Remove(std::pair<T, int> value) {
  auto it = std::find(elements_.begin(), elements_.end(), value);
  if (it != elements_.end()) {
    elements_.erase(it);
  }
}

template <typename T>
bool Bucket<T>::Has(T value) const {
  return std::find_if(elements_.begin(), elements_.end(),
                      [value](const std::pair<T, int>& pair) {
                        return pair.first == value;
                      }) != elements_.end();
}

template <typename T>
std::pair<T, int> Bucket<T>::Get(T value) const {
  auto it = std::find_if(
      elements_.begin(), elements_.end(),
      [value](const std::pair<T, int>& pair) { return pair.first == value; });
  if (it != elements_.end()) {
    return *it;
  }
  return std::pair<T, int>{value, 0};
}

class SimilarStringHashTable {
 public:
  SimilarStringHashTable(int size = kDefaultTableSize)
      : table_(size, Bucket<size_t>()) {}

  void Insert(const std::string& value);
  bool Has(const std::string& value) const;

 private:
  std::vector<Bucket<size_t>> table_;
};

void SimilarStringHashTable::Insert(const std::string& value) {
  auto value_full_hash = Hash(value);
  int hash = Hash(value) % table_.size();
  std::pair<size_t, int> searching_pair = {value_full_hash, 1};
  if (!table_[hash].Has(value_full_hash)) {
    table_[hash].Insert(searching_pair);
    return;
  }
  auto existing_pair = table_[hash].Get(value_full_hash);
  searching_pair.second = existing_pair.second + 1;
  table_[hash].Remove(existing_pair);
  table_[hash].Insert(searching_pair);
}

bool SimilarStringHashTable::Has(const std::string& value) const {
  int hash = Hash(value) % table_.size();
  return table_[hash].Has(Hash(value));
}

int main() {
  int amount_of_questions;
  std::cin >> amount_of_questions;
  SimilarStringHashTable table;
  for (int i = 0; i < amount_of_questions; ++i) {
    std::string command;
    std::string str;
    std::cin >> command >> str;
    if (command == "+") {
      table.Insert(str);
    } else {
      std::cout << (table.Has(str) ? "YES" : "NO") << "\n";
    }
  }
}
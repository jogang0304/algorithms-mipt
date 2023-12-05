#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

const int kDefaultTableSize = 1'000'000;

template <typename T>
class HashTable {
 public:
  HashTable(int size = kDefaultTableSize) : table_(size, std::set<T>()) {}

  void Insert(T value);
  void Remove(T value);
  bool Has(T value) const;

 private:
  std::vector<std::set<T>> table_;
};

template <typename T>
void HashTable<T>::Insert(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  if (std::find(table_[hash].begin(), table_[hash].end(), value) ==
      table_[hash].end()) {
    table_[hash].insert(value);
  }
}

template <typename T>
void HashTable<T>::Remove(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  auto it = std::find(table_[hash].begin(), table_[hash].end(), value);
  if (it != table_[hash].end()) {
    table_[hash].erase(it);
  }
}

template <typename T>
bool HashTable<T>::Has(T value) const {
  int hash = std::hash<T>{}(value) % table_.size();
  return std::find(table_[hash].begin(), table_[hash].end(), value) !=
         table_[hash].end();
}

int main() {
  int amount_of_questions;
  std::cin >> amount_of_questions;
  HashTable<int> table;
  for (int i = 0; i < amount_of_questions; ++i) {
    std::string command;
    int value;
    std::cin >> command >> value;
    if (command == "+") {
      table.Insert(value);
    } else if (command == "-") {
      table.Remove(value);
    } else {
      std::cout << (table.Has(value) ? "YES\n" : "NO\n");
    }
  }
}
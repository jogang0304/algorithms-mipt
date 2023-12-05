#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

const int kDefaultTableSize = 1'000'000;

template <typename T>
struct TablePair {
  T value;
  int count;
};

template <typename T>
bool TablePairCompare(const TablePair<T>& left, const TablePair<T>& right) {
  return left.value < right.value;
}

template <typename T>
class CountHashTable {
 public:
  CountHashTable(int size = kDefaultTableSize)
      : table_(size, std::set<TablePair<T>, decltype(TablePairCompare<T>)*>(
                         TablePairCompare<T>)) {}

  void Insert(T value);
  void Remove(T value);
  bool Has(T value) const;

 private:
  std::vector<std::set<TablePair<T>, decltype(TablePairCompare<T>)*>> table_;
};

template <typename T>
void CountHashTable<T>::Insert(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  auto searching_pair = TablePair<T>{value, 1};
  auto it = table_[hash].find(searching_pair);
  if (it == table_[hash].end()) {
    table_[hash].insert(searching_pair);
    return;
  }
  if (it->count > 0) {
    return;
  }
  searching_pair.count = it->count + 1;
  table_[hash].erase(it);
  table_[hash].insert(searching_pair);
}

template <typename T>
void CountHashTable<T>::Remove(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  auto searching_pair = TablePair<T>{value, 1};
  auto it = table_[hash].find(searching_pair);
  if (it == table_[hash].end()) {
    return;
  }
  searching_pair.count = it->count - 1;
  table_[hash].erase(it);
  if (searching_pair.count == 0) {
    return;
  }
  table_[hash].insert(searching_pair);
}

template <typename T>
bool CountHashTable<T>::Has(T value) const {
  int hash = std::hash<T>{}(value) % table_.size();
  auto searching_pair = TablePair<T>{value, 1};
  return table_[hash].find(searching_pair) != table_[hash].end();
}

int main() {
  int amount_of_questions;
  std::cin >> amount_of_questions;
  CountHashTable<int> table;
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
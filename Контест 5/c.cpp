#include <limits.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

const int kDefaultTableSize = 1'000'000;

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

struct TablePair {
  size_t value;
  int count;
};

bool TablePairCompare(const TablePair& left, const TablePair& right) {
  return left.value < right.value;
}

class SimilarStringHashTable {
 public:
  SimilarStringHashTable(int size = kDefaultTableSize)
      : table_(size, std::set<TablePair, decltype(TablePairCompare)*>(
                         TablePairCompare)) {}

  void Insert(const std::string& value);
  bool Has(const std::string& value) const;

 private:
  std::vector<std::set<TablePair, decltype(TablePairCompare)*>> table_;
};

void SimilarStringHashTable::Insert(const std::string& value) {
  int hash = Hash(value) % table_.size();
  auto searching_pair = TablePair{Hash(value), 1};
  auto it = table_[hash].find(searching_pair);
  if (it == table_[hash].end()) {
    table_[hash].insert(searching_pair);
    return;
  }
  searching_pair.count = it->count + 1;
  table_[hash].erase(it);
  table_[hash].insert(searching_pair);
}

bool SimilarStringHashTable::Has(const std::string& value) const {
  int hash = Hash(value) % table_.size();
  auto searching_pair = TablePair{Hash(value), 1};
  return table_[hash].find(searching_pair) != table_[hash].end();
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
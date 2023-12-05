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
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);

  int amount_of_ash_pokemon;
  std::cin >> amount_of_ash_pokemon;
  std::vector<int> ash_pokemon(amount_of_ash_pokemon);
  for (auto& pokemon : ash_pokemon) {
    std::cin >> pokemon;
  }
  int amount_of_misty_pokemon;
  std::cin >> amount_of_misty_pokemon;
  std::vector<int> misty_pokemon(amount_of_misty_pokemon);
  for (auto& pokemon : misty_pokemon) {
    std::cin >> pokemon;
  }

  std::vector<int> same_pokemon(0);
  CountHashTable<int> table;
  for (int i = 0; i < amount_of_ash_pokemon; ++i) {
    table.Insert(ash_pokemon[i]);
  }
  for (int i = 0; i < amount_of_misty_pokemon; ++i) {
    if (table.Has(misty_pokemon[i])) {
      same_pokemon.push_back(misty_pokemon[i]);
      table.Remove(misty_pokemon[i]);
    }
  }

  std::cout << same_pokemon.size() << '\n';
  for (auto pokemon : same_pokemon) {
    std::cout << pokemon << " ";
  }
}
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

const int kDefaultTableSize = 1'000'000;

// template <typename T>
// struct TablePair {
//   T value;
//   int count;
// };

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
  elements_.push_back(value);
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

template <typename T>
class CountHashTable {
 public:
  CountHashTable(int size = kDefaultTableSize) : table_(size, Bucket<T>()) {}

  void Insert(T value);
  void Remove(T value);
  bool Has(T value) const;

 private:
  std::vector<Bucket<T>> table_;
};

template <typename T>
void CountHashTable<T>::Insert(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  std::pair<T, int> searching_pair = {value, 1};
  if (!table_[hash].Has(value)) {
    table_[hash].Insert(searching_pair);
    return;
  }
  std::pair<T, int> existing_pair = table_[hash].Get(value);
  // if (existing_pair.second > 0) {
  //   return;
  // }
  searching_pair.second = existing_pair.second + 1;
  table_[hash].Remove(existing_pair);
  table_[hash].Insert(searching_pair);
}

template <typename T>
void CountHashTable<T>::Remove(T value) {
  int hash = std::hash<T>{}(value) % table_.size();
  std::pair<T, int> searching_pair = {value, 1};
  if (!table_[hash].Has(value)) {
    return;
  }
  std::pair<T, int> existing_pair = table_[hash].Get(value);
  searching_pair.second = existing_pair.second - 1;
  table_[hash].Remove(existing_pair);
  if (searching_pair.second == 0) {
    return;
  }
  table_[hash].Insert(searching_pair);
}

template <typename T>
bool CountHashTable<T>::Has(T value) const {
  int hash = std::hash<T>{}(value) % table_.size();
  auto searching_pair = std::pair<T, int>{value, 1};
  return table_[hash].Has(value);
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
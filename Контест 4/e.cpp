#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

struct PetrolStation {
  int x;
  int y_low;
  int y_high;
};

bool PetrolXCompare(const PetrolStation& left, const PetrolStation& right) {
  return (left.x < right.x) ||
         (left.x == right.x && left.y_low < right.y_low) ||
         (left.x == right.x && left.y_low == right.y_low &&
          left.y_high < right.y_high);
}

bool PetrolHighCompare(const PetrolStation& left, const PetrolStation& right) {
  return (left.y_high < right.y_high) ||
         (left.y_high == right.y_high && left.x < right.x) ||
         (left.y_high == right.y_high && left.x == right.x &&
          left.y_low < right.y_low);
}

bool PetrolLowCompare(const PetrolStation& left, const PetrolStation& right) {
  return (left.y_low < right.y_low) ||
         (left.y_low == right.y_low && left.x < right.x) ||
         (left.y_low == right.y_low && left.x == right.x &&
          left.y_high < right.y_high);
}

bool NumberReverseCompare(int left, int right) { return right < left; }

void HandleInsertStation(
    std::multiset<PetrolStation, decltype(PetrolLowCompare)*>&
        petrol_stations_by_low,
    std::multiset<PetrolStation, decltype(PetrolXCompare)*>& current_stations,
    std::multiset<int, decltype(NumberReverseCompare)*>& current_distances,
    int cur_index) {
  while (petrol_stations_by_low.begin() != petrol_stations_by_low.end() &&
         petrol_stations_by_low.begin()->y_low == cur_index) {
    auto it = *petrol_stations_by_low.begin();
    current_stations.insert(it);
    petrol_stations_by_low.erase(it);
    auto next = *(++current_stations.find(it));
    if (current_stations.find(it) == (--current_stations.end())) {
      next = *(--current_stations.end());
    }
    auto prev = *(--current_stations.find(it));
    if (current_stations.find(it) == current_stations.begin()) {
      prev = *current_stations.begin();
    }
    int prev_distance = next.x - prev.x;
    int new_distance_1 = it.x - prev.x;
    int new_distance_2 = next.x - it.x;
    current_distances.erase(prev_distance);
    current_distances.insert(new_distance_1);
    current_distances.insert(new_distance_2);
  }
}

void HandleDeleteStation(
    std::multiset<PetrolStation, decltype(PetrolHighCompare)*>&
        petrol_stations_by_high,
    std::multiset<PetrolStation, decltype(PetrolXCompare)*>& current_stations,
    std::multiset<int, decltype(NumberReverseCompare)*>& current_distances,
    int cur_index) {
  while (petrol_stations_by_high.begin() != petrol_stations_by_high.end() &&
         petrol_stations_by_high.begin()->y_high == cur_index - 1) {
    auto it = *petrol_stations_by_high.begin();
    auto next = *(++current_stations.find(it));
    if (current_stations.find(it) == (--current_stations.end())) {
      next = *(--current_stations.end());
    }
    auto prev = *(--current_stations.find(it));
    if (current_stations.find(it) == current_stations.begin()) {
      prev = *current_stations.begin();
    }
    int prev_distance_1 = it.x - prev.x;
    int prev_distance_2 = next.x - it.x;
    int new_distance = next.x - prev.x;
    current_distances.erase(prev_distance_1);
    current_distances.erase(prev_distance_2);
    current_distances.insert(new_distance);
    current_stations.erase(it);
    petrol_stations_by_high.erase(it);
  }
}

int main() {
  int n_value;
  int x_value;
  int y_value;
  std::cin >> n_value >> y_value >> x_value;
  std::multiset<PetrolStation, decltype(PetrolHighCompare)*>
      petrol_stations_by_high(PetrolHighCompare);
  std::multiset<PetrolStation, decltype(PetrolLowCompare)*>
      petrol_stations_by_low(PetrolLowCompare);
  for (int i = 0; i < n_value; ++i) {
    int cur_x;
    int cur_y1;
    int cur_y2;
    std::cin >> cur_x >> cur_y1 >> cur_y2;
    PetrolStation cur_station = {cur_x, cur_y1, cur_y2};
    petrol_stations_by_high.insert(cur_station);
    petrol_stations_by_low.insert(cur_station);
  }
  std::multiset<PetrolStation, decltype(PetrolXCompare)*> current_stations(
      PetrolXCompare);
  current_stations.insert({0, 0, y_value});
  current_stations.insert({x_value, 0, y_value});
  std::multiset<int, decltype(NumberReverseCompare)*> current_distances(
      NumberReverseCompare);
  current_distances.insert(x_value);
  for (int i = 0; i <= y_value; ++i) {
    if ((!petrol_stations_by_low.empty() &&
         petrol_stations_by_low.begin()->y_low == i) ||
        (!petrol_stations_by_high.empty() &&
         petrol_stations_by_high.begin()->y_high == i - 1)) {
      HandleDeleteStation(petrol_stations_by_high, current_stations,
                          current_distances, i);
      HandleInsertStation(petrol_stations_by_low, current_stations,
                          current_distances, i);
    }
    std::cout << *current_distances.begin() << "\n";
  }
}
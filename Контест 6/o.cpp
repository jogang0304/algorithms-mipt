#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2e9;
const int kMaxCities = 15;

int min_length = kInf;
int amount_of_cities = 0;
int cur_path[kMaxCities];
bool used_cities[kMaxCities];
int min_route_cities[kMaxCities];
std::vector<std::vector<int>> path_lengths = {};

void Dfs(int cur_used_amount_of_cities, int cur_answer, int cur_max_path) {
  if (cur_used_amount_of_cities == amount_of_cities) {
    cur_max_path = std::max(
        cur_max_path, path_lengths[cur_path[cur_used_amount_of_cities - 1]][0]);
    cur_answer +=
        path_lengths[cur_path[cur_used_amount_of_cities - 1]][0] - cur_max_path;
    if (cur_answer < min_length) {
      min_length = cur_answer;
      for (int i = 0; i < amount_of_cities; ++i) {
        min_route_cities[i] = cur_path[i];
      }
    }
    return;
  }
  for (int i = 0; i < amount_of_cities; ++i) {
    if (used_cities[i]) {
      continue;
    }
    int new_cur_answer =
        cur_answer + path_lengths[cur_path[cur_used_amount_of_cities - 1]][i];
    int new_cur_max_path = std::max(
        cur_max_path, path_lengths[cur_path[cur_used_amount_of_cities - 1]][i]);
    if (new_cur_answer - new_cur_max_path < min_length) {
      used_cities[i] = true;
      cur_path[cur_used_amount_of_cities] = i;
      Dfs(cur_used_amount_of_cities + 1, new_cur_answer, new_cur_max_path);
      used_cities[i] = false;
    }
  }
}

int main() {
  std::cin >> amount_of_cities;
  path_lengths.resize(amount_of_cities, std::vector<int>(amount_of_cities));
  for (int i = 0; i < amount_of_cities; ++i) {
    for (int j = 0; j < amount_of_cities; ++j) {
      std::cin >> path_lengths[i][j];
    }
  }
  for (int i = 0; i < kMaxCities; ++i) {
    cur_path[i] = 0;
    used_cities[i] = false;
  }
  used_cities[0] = true;
  Dfs(1, 0, 0);
  std::cout << min_length << "\n";
  int max_path = 0;
  int max_path_start_index = 0;
  for (int i = 0; i < amount_of_cities; ++i) {
    if (path_lengths[min_route_cities[i]]
                    [min_route_cities[(i + 1) % amount_of_cities]] > max_path) {
      max_path = path_lengths[min_route_cities[i]]
                             [min_route_cities[(i + 1) % amount_of_cities]];
      max_path_start_index = i;
    }
  }
  for (int i = max_path_start_index + 1; i < amount_of_cities; ++i) {
    std::cout << min_route_cities[i] + 1 << " ";
  }
  for (int i = 0; i <= max_path_start_index; ++i) {
    std::cout << min_route_cities[i] + 1 << " ";
  }
}
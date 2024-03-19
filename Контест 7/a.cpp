#include <algorithm>
#include <iostream>
#include <vector>

std::vector<int> colors;
std::vector<int> current_path(0);
std::vector<std::vector<int>> paths;

std::vector<int> MakeCycle(std::vector<int>& path, int start_value) {
  std::vector<int> cycle(0);
  size_t cycle_point_index = 0;
  while (path[cycle_point_index] != start_value) {
    ++cycle_point_index;
  }
  for (size_t i = cycle_point_index; i < path.size(); ++i) {
    cycle.push_back(path[i]);
  }
  return cycle;
}

std::vector<int> DfsFindCycle() {
  int current_point = current_path.back();
  for (size_t i = 0; i < paths[current_point].size(); ++i) {
    int next_point = paths[current_point][i];
    if (colors[next_point] == 2) {
      continue;
    }
    if (colors[next_point] == 1) {
      return MakeCycle(current_path, next_point);
    }
    current_path.push_back(next_point);
    colors[current_point] = 1;
    auto cycle = DfsFindCycle();
    colors[current_point] = 2;
    current_path.pop_back();
    if (!cycle.empty()) {
      return cycle;
    }
  }
  return {};
}

int main() {
  int amount_of_points;
  int amount_of_paths;
  std::cin >> amount_of_points >> amount_of_paths;
  colors.resize(amount_of_points, 0);  // 0 - white, 1 - gray, 2 - black
  current_path.resize(0);
  paths.resize(amount_of_points, std::vector<int>(0, 0));
  for (int i = 0; i < amount_of_paths; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    paths[start - 1].push_back(end - 1);
  }
  std::vector<int> cycle(0);
  for (size_t i = 0; i < colors.size(); ++i) {
    if (colors[i] == 0) {
      current_path.push_back(static_cast<int>(i));
      colors[i] = 1;
      cycle = DfsFindCycle();
      colors[i] = 2;
      current_path.pop_back();
      if (!cycle.empty()) {
        break;
      }
    }
  }
  if (!cycle.empty()) {
    std::cout << "YES\n";
    for (auto element : cycle) {
      std::cout << element + 1 << " ";
    }
  } else {
    std::cout << "NO";
  }
}
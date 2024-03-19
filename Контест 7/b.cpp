#include <iostream>
#include <set>
#include <vector>

void Dfs(std::vector<std::vector<int>>& pairs, std::vector<int>& colors,
         int clone, int color) {
  colors[clone] = color;
  for (auto bind_clone : pairs[clone]) {
    if (colors[bind_clone] == -1) {
      Dfs(pairs, colors, bind_clone, color);
    }
  }
}

int main() {
  int amount_of_clones;
  int amount_of_pairs;
  std::cin >> amount_of_clones >> amount_of_pairs;
  std::vector<std::vector<int>> pairs(amount_of_clones, std::vector<int>(0, 0));
  for (int i = 0; i < amount_of_pairs; i++) {
    int first;
    int second;
    std::cin >> first >> second;
    pairs[first - 1].push_back(second - 1);
    pairs[second - 1].push_back(first - 1);
  }
  std::vector<int> colors(amount_of_clones, -1);
  int color = 0;
  std::set<int> cur_set;
  for (int i = 0; i < amount_of_clones; i++) {
    if (colors[i] != -1) {
      continue;
    }
    Dfs(pairs, colors, i, color++);
  }
  std::vector<std::vector<int>> groups(color, std::vector<int>(0, 0));
  for (int i = 0; i < amount_of_clones; i++) {
    groups[colors[i]].push_back(i);
  }
  std::cout << groups.size() << "\n";
  for (const auto& group : groups) {
    std::cout << group.size() << "\n";
    for (auto clone : group) {
      std::cout << clone + 1 << " ";
    }
    std::cout << "\n";
  }
}
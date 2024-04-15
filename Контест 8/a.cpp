#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int cInf = 2'009'000'999;

void OneBlock() {
  int n;
  int m;
  std::cin >> n >> m;
  std::vector<std::vector<std::pair<int, int>>> graph(
      n, std::vector<std::pair<int, int>>(0));
  for (int i = 0; i < m; ++i) {
    int from;
    int to;
    int weight;
    std::cin >> from >> to >> weight;
    graph[from].push_back({to, weight});
    graph[to].push_back({from, weight});
  }
  int start;
  std::cin >> start;
  std::vector<int> distances(n, cInf);
  distances[start] = 0;
  std::set<std::pair<int, int>> queue;
  queue.insert({0, start});
  while (!queue.empty()) {
    auto b = *queue.begin();
    queue.erase(queue.begin());
    int v = b.second;
    for (auto u : graph[v]) {
      if (distances[u.first] > distances[v] + u.second) {
        queue.erase({distances[u.first], u.first});
        distances[u.first] = distances[v] + u.second;
        queue.insert({distances[u.first], u.first});
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    std::cout << distances[i] << " ";
  }
  std::cout << "\n";
}

int main() {
  int k;
  std::cin >> k;
  for (int i = 0; i < k; ++i) {
    OneBlock();
  }
}
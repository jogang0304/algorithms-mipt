#include <iostream>
#include <set>
#include <vector>

const int cInf = 2'000'000'000;
const int cLevels = 1'000'000;

int Dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph,
             int n) {
  std::vector<int> distances(graph.size(), cInf);
  distances[0] = 0;
  std::set<std::pair<int, int>> queue;  // {distance, index}
  queue.insert({0, 0});
  while (!queue.empty()) {
    auto cur = *queue.begin();
    queue.erase(queue.begin());
    for (const auto& edge : graph[cur.second]) {
      if (distances[edge.first] > cur.first + edge.second) {
        queue.erase({distances[edge.first], edge.first});
        distances[edge.first] = cur.first + edge.second;
        queue.insert({distances[edge.first], edge.first});
      }
    }
  }
  return distances[n];
}

int main() {
  int n;
  int u;
  int d;
  int i;
  int j;
  int l;
  std::cin >> n >> u >> d >> i >> j >> l;
  std::vector<std::vector<std::pair<int, int>>> graph(
      cLevels + l);  // {destination, time}
  for (int a = 0; a + 1 < cLevels; ++a) {
    graph[a].push_back({a + 1, u});
    graph[a + 1].push_back({a, d});
  }
  for (int a = 0; a < l; ++a) {
    int k;
    std::cin >> k;
    for (int b = 0; b < k; ++b) {
      int cur_tp;
      std::cin >> cur_tp;
      graph[cur_tp - 1].push_back({cLevels + a, i});
      graph[cLevels + a].push_back({cur_tp - 1, j});
    }
  }
  auto ans = Dijkstra(graph, n - 1);
  std::cout << ans;
}
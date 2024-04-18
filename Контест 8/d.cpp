#include <algorithm>
#include <iostream>
#include <vector>

const int cInf = 30'000;

int main() {
  int n;
  int m;
  std::cin >> n >> m;
  std::vector<std::vector<std::pair<int, int>>> graph(n);
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int weight;
    std::cin >> start >> end >> weight;
    graph[start - 1].push_back({end - 1, weight});
  }
  std::vector<int> d(n, cInf);
  d[0] = 0;
  for (int iteration = 0; iteration < n; ++iteration) {
    for (int i = 0; i < n; ++i) {
      for (auto& path : graph[i]) {
        if ((d[i] < cInf) && (d[i] + path.second < d[path.first])) {
          d[path.first] = d[i] + path.second;
        }
      }
    }
  }
  for (auto x : d) {
    std::cout << x << " ";
  }
}
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

const int cSmallInf = 100'000;
const int cInf = 1'000'000'000;

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> graph(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> graph[i][j];
      if (graph[i][j] == cSmallInf) {
        graph[i][j] = cInf;
      }
    }
  }
  std::vector<int> d(n, cInf);
  std::vector<int> came_from(n, cInf);
  bool has_negative_cycle = false;
  int bad_node = -1;
  for (int start = 0; start < n; ++start) {
    if (d[start] != cInf) {
      continue;
    }
    d[start] = 0;
    for (int iteration = 0; iteration < n; ++iteration) {
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (d[i] + graph[i][j] < d[j]) {
            d[j] = d[i] + graph[i][j];
            came_from[j] = i;
          }
        }
      }
    }
  }
  for (int i = 0; (i < n) && (!has_negative_cycle); ++i) {
    for (int j = 0; (j < n) && (!has_negative_cycle); ++j) {
      if (d[i] + graph[i][j] < d[j]) {
        has_negative_cycle = true;
        bad_node = i;
      }
    }
  }
  if (has_negative_cycle) {
    std::cout << "YES\n";
    std::deque<int> nodes;
    std::vector<bool> visited(n, false);
    nodes.push_back(bad_node);
    visited[bad_node] = true;
    while (true) {
      bad_node = came_from[bad_node];
      nodes.push_back(bad_node);
      if (visited[bad_node]) {
        break;
      }
      visited[bad_node] = true;
    }
    std::cout << nodes.size() << "\n";
    for (int i = nodes.size() - 1; i >= 0; --i) {
      std::cout << nodes[i] + 1 << " ";
    }
  } else {
    std::cout << "NO";
  }
}
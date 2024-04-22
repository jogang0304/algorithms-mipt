#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <vector>

const int cInf = 2'000'000'000;

struct Path {
  int destination;
  int delta;
  int index;
};

struct Node {
  int distance = -cInf;
  int prev_node = cInf;
  int prev_path_index = cInf;
};

std::vector<int> GetNegativeCycle(const std::vector<std::vector<Path>>& graph,
                                  const std::multiset<int>& bad_nodes,
                                  int end) {
  std::vector<std::vector<int>> matrix(graph.size(),
                                       std::vector<int>(graph.size(), -cInf));
  for (size_t i = 0; i < graph.size(); ++i) {
    matrix[i][i] = 0;
    for (auto path : graph[i]) {
      matrix[i][path.destination] = path.delta;
    }
  }

  std::vector<std::vector<int>> next_node_to_get_to(
      matrix.size(), std::vector<int>(matrix.size(), -1));
  for (size_t i = 0; i < next_node_to_get_to.size(); ++i) {
    for (size_t j = 0; j < next_node_to_get_to.size(); ++j) {
      next_node_to_get_to[i][j] = static_cast<int>(j);
    }
  }
  bool found_positive_cycle = false;
  for (size_t k = 0; (k < matrix.size()) && !found_positive_cycle; ++k) {
    if (bad_nodes.find(k) != bad_nodes.end() && k != end) {
      continue;
    }
    for (size_t i = 0; (i < matrix.size()) && !found_positive_cycle; ++i) {
      for (size_t j = 0; (j < matrix.size()) && !found_positive_cycle; ++j) {
        if (matrix[i][j] >= matrix[i][k] + matrix[k][j]) {
          continue;
        }
        if (bad_nodes.find(j) != bad_nodes.end() && j != end) {
          continue;
        }
        matrix[i][j] = matrix[i][k] + matrix[k][j];
        next_node_to_get_to[i][j] = next_node_to_get_to[i][k];
        if (matrix[j][j] > 0) {
          found_positive_cycle = true;
          break;
        }
      }
    }
  }
  std::vector<int> ans(0);
  for (int i = 0; i < static_cast<int>(matrix.size()); ++i) {
    if (matrix[i][i] <= 0) {
      continue;
    }
    ans.push_back(i);
    int cur = next_node_to_get_to[i][i];
    while (cur != i) {
      ans.push_back(cur);
      cur = next_node_to_get_to[cur][i];
    }
    ans.push_back(cur);
    break;
  }
  return ans;
}

int main() {
  int n;
  int m;
  int k;
  std::cin >> n >> m >> k;
  std::vector<std::vector<Path>> graph(n);
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int d;
    std::cin >> start >> end >> d;
    graph[start - 1].push_back({end - 1, d, i});
  }
  std::multiset<int> bad_nodes;
  std::queue<int> a;
  for (int i = 0; i < k; ++i) {
    int cur_a;
    std::cin >> cur_a;
    --cur_a;
    a.push(cur_a);
    bad_nodes.insert(cur_a);
  }
  std::list<int> ans(0);
  while (a.size() >= 2) {
    int start = a.front();
    bad_nodes.erase(bad_nodes.find(start));
    a.pop();
    int end = a.front();
    std::pair<bool, std::list<int>> res =
        Dijkstra(graph, start, end, bad_nodes);
    if (res.first) {
      std::cout << "infinitely kind";
      return 0;
    }
    ans.splice(ans.end(), res.second);
  }
  std::cout << ans.size() << "\n";
  for (int x : ans) {
    std::cout << x + 1 << " ";
  }
}
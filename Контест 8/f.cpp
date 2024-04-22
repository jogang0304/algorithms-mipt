#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <vector>

const int cInf = 2'000'000'000;
const int cSmallInf = 1'000'000'000;

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

bool HasPositiveCycle(const std::vector<std::vector<Path>>& graph,
                      const std::multiset<int>& bad_nodes, int end) {
  std::vector<std::vector<int>> matrix(
      graph.size(), std::vector<int>(graph.size(), -cSmallInf));
  for (size_t i = 0; i < graph.size(); ++i) {
    matrix[i][i] = 0;
    if (bad_nodes.find(static_cast<int>(i)) != bad_nodes.end() &&
        static_cast<int>(i) != end) {
      continue;
    }
    for (auto path : graph[i]) {
      if (bad_nodes.find(path.destination) != bad_nodes.end() &&
          path.destination != end) {
        continue;
      }
      matrix[i][path.destination] = path.delta;
    }
  }
  bool found_positive_cycle = false;
  for (size_t k = 0; (k < matrix.size()) && !found_positive_cycle; ++k) {
    for (size_t i = 0; (i < matrix.size()) && !found_positive_cycle; ++i) {
      for (size_t j = 0; (j < matrix.size()) && !found_positive_cycle; ++j) {
        if (matrix[i][k] == -cSmallInf || matrix[k][j] == -cSmallInf ||
            matrix[i][j] >= matrix[i][k] + matrix[k][j]) {
          continue;
        }
        matrix[i][j] = matrix[i][k] + matrix[k][j];

        if (matrix[j][j] > 0) {
          found_positive_cycle = true;
          break;
        }
      }
    }
  }
  return found_positive_cycle;
}

// {is_infinite, path}
std::pair<bool, std::list<int>> Dijkstra(
    const std::vector<std::vector<Path>>& graph, int start, int end,
    const std::multiset<int>& bad_nodes) {
  if (HasPositiveCycle(graph, bad_nodes, end)) {
    return {true, {}};
  }
  std::vector<Node> distances(graph.size());
  distances[start].distance = 0;
  std::set<std::pair<int, int>, std::greater<std::pair<int, int>>> queue;
  queue.insert({0, start});
  while (!queue.empty()) {
    auto cur = *queue.begin();
    queue.erase(queue.begin());
    for (auto path : graph[cur.second]) {
      auto found = bad_nodes.find(path.destination);
      if (cur.first + path.delta > distances[path.destination].distance &&
          (found == bad_nodes.end() || (*found) == end)) {
        queue.erase({distances[path.destination].distance, path.destination});
        distances[path.destination].distance = cur.first + path.delta;
        distances[path.destination].prev_node = cur.second;
        distances[path.destination].prev_path_index = path.index;
        queue.insert({distances[path.destination].distance, path.destination});
      }
    }
  }
  std::list<int> ans(0);
  int cur_index = end;
  while (cur_index != start) {
    ans.push_front(distances[cur_index].prev_path_index);
    cur_index = distances[cur_index].prev_node;
  }
  return {false, ans};
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
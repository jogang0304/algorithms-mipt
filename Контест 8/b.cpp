#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int64_t cInf = 2'000'000'000'000'000'000;

struct Path {
  int destination = -1;
  int64_t distance = cInf;
};

struct Node {
  int64_t distance = cInf;
  bool is_infected = false;
  bool operator<(const Node& other) const {
    if (distance == other.distance) {
      return is_infected;
    }
    return distance < other.distance;
  }
};

std::vector<Node> Dijkstra(const std::vector<std::vector<Path>>& graph, int s,
                           const std::vector<int>& infected_nodes) {
  std::vector<Node> ans(graph.size());
  std::set<std::pair<Node, int>> queue;
  ans[s].distance = 0;
  queue.insert({ans[s], s});
  for (size_t i = 0; i < infected_nodes.size(); ++i) {
    ans[infected_nodes[i]].is_infected = true;
    ans[infected_nodes[i]].distance = 0;
    queue.insert({ans[infected_nodes[i]], infected_nodes[i]});
  }
  while (!queue.empty()) {
    auto current = *queue.begin();
    queue.erase(queue.begin());
    for (const Path& path : graph[current.second]) {
      if (ans[path.destination].distance >
          current.first.distance + path.distance) {
        queue.erase({ans[path.destination], path.destination});
        ans[path.destination].distance = current.first.distance + path.distance;
        ans[path.destination].is_infected = current.first.is_infected;
        queue.insert({ans[path.destination], path.destination});
      }
    }
  }
  return ans;
}

int main() {
  int n;
  int m;
  int k;
  std::cin >> n >> m >> k;
  std::vector<int> infected_nodes(k);
  for (int i = 0; i < k; ++i) {
    std::cin >> infected_nodes[i];
    infected_nodes[i]--;
  }
  std::vector<std::vector<Path>> graph(n);
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int64_t dist;
    std::cin >> start >> end >> dist;
    graph[start - 1].push_back({end - 1, dist});
    graph[end - 1].push_back({start - 1, dist});
  }
  int s;
  int t;
  std::cin >> s >> t;
  auto res = Dijkstra(graph, s - 1, infected_nodes);
  if (res[t - 1].is_infected || res[t - 1].distance == cInf) {
    std::cout << -1;
  } else {
    std::cout << res[t - 1].distance;
  }
}
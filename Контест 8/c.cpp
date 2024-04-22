#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int64_t cInf = 2'000'000'000;

struct Path {
  size_t destination = cInf;
  int64_t price = cInf;
  int64_t time = cInf;
};

struct Node {
  int64_t time = cInf;
  int64_t price = cInf;
  size_t index = cInf;
  size_t prev_index = cInf;
  int64_t prev_path_time = cInf;
};

bool NodeCmpByTime(const Node& left, const Node& right) {
  return left.time < right.time;
}

bool NodeCmpByPrice(const Node& left, const Node& right) {
  if (left.price != right.price) {
    return left.price < right.price;
  }
  return left.index < right.index;
}

std::pair<int64_t, std::vector<size_t>> GetMinPath(
    const std::vector<std::set<Node, decltype(NodeCmpByTime)*>>& nodes) {
  auto cur_node = --(--((nodes.back()).end()));
  int64_t min_price = cur_node->price;
  if (min_price == cInf) {
    return {-1, {}};
  }
  std::vector<size_t> ans_path(0);
  while (cur_node->index != 0) {
    ans_path.push_back(cur_node->index);
    cur_node =
        --(nodes[cur_node->prev_index].upper_bound({cur_node->prev_path_time}));
  }
  ans_path.push_back(0);
  std::reverse(ans_path.begin(), ans_path.end());
  return {min_price, ans_path};
}

auto Dijkstra(const std::vector<std::vector<Path>>& graph, int t) {
  std::vector<std::set<Node, decltype(NodeCmpByTime)*>> nodes(
      graph.size(), std::set<Node, decltype(NodeCmpByTime)*>(NodeCmpByTime));
  nodes[0].insert({0, 0, 0});
  for (size_t i = 1; i < graph.size(); ++i) {
    nodes[i].insert({0, cInf, i});
    nodes[i].insert({cInf, cInf, i});
  }
  std::set<Node, decltype(NodeCmpByPrice)*> queue(NodeCmpByPrice);
  queue.insert({0, 0, 0});
  while (!queue.empty()) {
    Node current = *queue.begin();
    queue.erase(queue.begin());
    for (auto path : graph[current.index]) {
      int64_t new_price = current.price + path.price;
      int64_t new_time = current.time + path.time;
      if (new_time > t) {
        continue;
      }
      auto best_passing_time_it =
          --(nodes[path.destination].upper_bound({new_time, new_price, cInf}));
      if (new_price < best_passing_time_it->price) {
        if (new_time == best_passing_time_it->time) {
          queue.erase(*best_passing_time_it);
          nodes[path.destination].erase(best_passing_time_it);
        }
        nodes[path.destination].insert(
            {new_time, new_price, path.destination, current.index, path.time});
        queue.insert({new_time, new_price, path.destination});
      }
    }
  }
  return GetMinPath(nodes);
}

int main() {
  int n;
  int m;
  int t;
  std::cin >> n >> m >> t;
  std::vector<std::vector<Path>> graph(n);
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int price;
    int time;
    std::cin >> start >> end >> price >> time;
    graph[start - 1].push_back({static_cast<size_t>(end - 1), price, time});
    graph[end - 1].push_back({static_cast<size_t>(start - 1), price, time});
  }
  auto res = Dijkstra(graph, t);
  std::cout << res.first << "\n";
  if (res.first != -1) {
    std::cout << res.second.size() << "\n";
    for (auto x : res.second) {
      std::cout << x + 1 << " ";
    }
  }
}
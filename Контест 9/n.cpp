#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

struct Edge {
  size_t destination = 0;
  int64_t capacity = 0;
  bool operator<(const Edge& other) const {
    return destination < other.destination;
  }
};

class FlowSolver {
 public:
  FlowSolver(size_t s, size_t t, std::vector<std::set<Edge>>& graph,
             int64_t max_capacity)
      : start_(s), end_(t), graph_(graph), max_capacity_(max_capacity) {}

  struct Flows {
    int64_t value;
    std::vector<std::vector<int64_t>> flows;
  };

  Flows GetMaxFlow() {
    std::vector<std::vector<int64_t>> flows(
        graph_.size(), std::vector<int64_t>(graph_.size(), 0));
    EdmondsKarp(flows);
    int64_t ans = 0;
    for (size_t i = 0; i < graph_.size(); ++i) {
      ans += flows[i][end_];
    }
    return {ans, flows};
  }

 private:
  size_t start_;
  size_t end_;
  std::vector<std::set<Edge>> graph_;
  int64_t max_capacity_;

  void EdmondsKarp(std::vector<std::vector<int64_t>>& flows) {
    int64_t resize_value = max_capacity_ - 1;
    while (true) {
      auto shortest_path = GetShortestPath(flows, resize_value);
      if (shortest_path.path.size() < 2) {
        if (resize_value == 0) {
          break;
        }
        resize_value = std::max(int64_t(0), resize_value / 2);
        continue;
      }
      int64_t min_capacity = shortest_path.min_capacity_on_path;
      for (size_t i = 0; i + 1 < shortest_path.path.size(); ++i) {
        flows[shortest_path.path[i]][shortest_path.path[i + 1]] += min_capacity;
        flows[shortest_path.path[i + 1]][shortest_path.path[i]] -= min_capacity;
      }
      resize_value = std::max(int64_t(0), resize_value / 2);
    }
  }

  struct ShortestPath {
    std::vector<size_t> path = {};
    int64_t min_capacity_on_path = 1'000'000'000'000;
  };
  ShortestPath GetShortestPath(const std::vector<std::vector<int64_t>>& flows,
                               int64_t resize_value) {
    std::queue<size_t> queue;
    std::vector<bool> used(graph_.size(), false);
    queue.push(start_);
    used[start_] = true;
    std::vector<std::pair<size_t, int64_t>> prev(graph_.size());
    prev[start_].first = start_;
    while (!queue.empty()) {
      auto cur = queue.front();
      queue.pop();
      for (auto x : graph_[cur]) {
        int64_t left_capacity = x.capacity - flows[cur][x.destination];
        if (left_capacity > resize_value && !used[x.destination]) {
          queue.push(x.destination);
          used[x.destination] = true;
          prev[x.destination].first = cur;
          prev[x.destination].second = left_capacity;
        }
      }
    }
    if (!used[end_]) {
      return {};
    }
    ShortestPath ans;
    size_t cur = end_;
    while (cur != start_) {
      ans.path.push_back(cur);
      ans.min_capacity_on_path =
          std::min(ans.min_capacity_on_path, prev[cur].second);
      cur = prev[cur].first;
    }
    ans.path.push_back(start_);
    std::reverse(ans.path.begin(), ans.path.end());
    return ans;
  }
};

int main() {
  int64_t n;
  int64_t m;
  std::cin >> n >> m;
  std::vector<std::set<Edge>> caps(n);
  std::vector<std::vector<std::vector<int64_t>>> map_cords_to_index(
      n, std::vector<std::vector<int64_t>>(n, std::vector<int64_t>(0)));
  std::vector<int64_t> edges_caps(m);
  std::vector<int64_t> edges_fill(m, 0);
  int64_t max_capacity = 0;
  for (int64_t i = 0; i < m; ++i) {
    size_t start;
    size_t end;
    int64_t cap;
    std::cin >> start >> end >> cap;
    auto present = caps[start - 1].find({end - 1, 0});
    if (present == caps[start - 1].end()) {
      caps[start - 1].insert({end - 1, cap});
      max_capacity = std::max(max_capacity, cap);
    } else {
      Edge new_edge = {end - 1, present->capacity + cap};
      caps[start - 1].erase(present);
      caps[start - 1].insert(new_edge);
      max_capacity = std::max(max_capacity, new_edge.capacity);
    }
    map_cords_to_index[start - 1][end - 1].push_back(i);
    edges_caps[i] = cap;
  }
  FlowSolver solver(0, n - 1, caps, max_capacity);
  auto ans = solver.GetMaxFlow();
  std::cout << ans.value << "\n";
  for (size_t i = 0; i < ans.flows.size(); ++i) {
    for (size_t j = 0; j < ans.flows[i].size(); ++j) {
      int64_t left_to_use = ans.flows[i][j];
      auto indexes = map_cords_to_index[i][j];
      size_t k = 0;
      while (left_to_use > 0) {
        int64_t used = std::min(edges_caps[indexes[k]] - edges_fill[indexes[k]],
                                left_to_use);
        edges_fill[indexes[k]] += used;
        left_to_use -= used;
        ++k;
      }
    }
  }
  for (auto x : edges_fill) {
    std::cout << x << "\n";
  }
}
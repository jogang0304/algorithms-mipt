#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

const int64_t cInf = 1'000'000'000'000'000'000;

class FlowSolver {
 public:
  FlowSolver(size_t s, size_t t, std::vector<std::vector<int64_t>>& caps,
             int64_t max_capacity)
      : start_(s), end_(t), capacities_(caps), max_capacity_(max_capacity) {}

  struct Flows {
    int64_t value;
    std::vector<std::vector<int64_t>> flows;
  };

  Flows GetMaxFlow() {
    std::vector<std::vector<int64_t>> flows(
        capacities_.size(), std::vector<int64_t>(capacities_.size(), 0));
    int64_t ans = EdmondsKarp(flows);
    return {ans, flows};
  }

 private:
  size_t start_;
  size_t end_;
  std::vector<std::vector<int64_t>> capacities_;
  int64_t max_capacity_;

  int64_t EdmondsKarp(std::vector<std::vector<int64_t>>& flows) {
    int64_t flow_to_end = 0;
    int64_t resize_value = max_capacity_ - 1;
    while (true) {
      auto shortest_path_reversed =
          GetShortestPathReversed(flows, resize_value);
      if (shortest_path_reversed.path.size() < 2) {
        if (resize_value == 0) {
          break;
        }
        resize_value = std::max(int64_t(0), resize_value / 2);
        continue;
      }
      int64_t value_to_change = shortest_path_reversed.min_left_capacity;
      for (size_t i = 0; i + 1 < shortest_path_reversed.path.size(); ++i) {
        flows[shortest_path_reversed.path[i]]
             [shortest_path_reversed.path[i + 1]] -= value_to_change;
        flows[shortest_path_reversed.path[i + 1]]
             [shortest_path_reversed.path[i]] += value_to_change;
      }
      flow_to_end += value_to_change;
      resize_value = std::max(int64_t(0), resize_value / 2);
    }
    return flow_to_end;
  }

  struct ShortestPathInfo {
    std::vector<size_t> path = {};
    int64_t min_left_capacity = cInf;
  };

  ShortestPathInfo GetShortestPathReversed(
      const std::vector<std::vector<int64_t>>& flows, int64_t resize_value) {
    std::queue<size_t> queue;
    std::vector<bool> used(capacities_.size(), false);
    queue.push(start_);
    used[start_] = true;
    std::vector<size_t> prev(capacities_.size(), 0);
    prev[start_] = start_;
    while (!queue.empty()) {
      auto cur = queue.front();
      queue.pop();
      for (size_t i = 0; i < capacities_[cur].size(); ++i) {
        int64_t left_capacity = capacities_[cur][i] - flows[cur][i];
        if (left_capacity > resize_value && !used[i]) {
          queue.push(i);
          used[i] = true;
          prev[i] = cur;
        }
      }
    }
    if (!used[end_]) {
      return {};
    }

    ShortestPathInfo ans;
    size_t cur = end_;
    while (cur != start_) {
      ans.path.push_back(cur);
      ans.min_left_capacity =
          std::min(ans.min_left_capacity,
                   capacities_[prev[cur]][cur] - flows[prev[cur]][cur]);
      cur = prev[cur];
    }
    ans.path.push_back(start_);
    return ans;
  }
};

int main() {
  int64_t n;
  int64_t m;
  std::cin >> n >> m;
  std::vector<std::vector<int64_t>> caps(n, std::vector<int64_t>(n, 0));
  std::vector<std::vector<std::vector<int64_t>>> map_cords_to_index(
      n, std::vector<std::vector<int64_t>>(n, std::vector<int64_t>(0)));
  std::vector<int64_t> edges_caps(m);
  std::vector<int64_t> edges_fill(m, 0);
  int64_t max_capacity = 0;
  for (int64_t i = 0; i < m; ++i) {
    int64_t start;
    int64_t end;
    int64_t cap;
    std::cin >> start >> end >> cap;
    caps[start - 1][end - 1] += cap;
    map_cords_to_index[start - 1][end - 1].push_back(i);
    edges_caps[i] = cap;
    max_capacity = std::max(max_capacity, caps[start - 1][end - 1]);
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
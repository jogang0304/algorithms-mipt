#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

struct Edge {
 public:
  int capacity = 0;
  int index = 0;
};

class FlowSolver {
 public:
  FlowSolver(size_t s, size_t t, std::vector<std::vector<Edge>>& caps)
      : start_(s), end_(t), capacities_(caps) {}

  struct Cut {
    int value = 0;
    std::vector<int> deleted_edges_indexes = {};
  };

  Cut GetMinCut() {
    auto is_reachable = GetReachableMap(GetFlowsEdmondsKarp());
    Cut ans;
    for (size_t i = 0; i < capacities_.size(); ++i) {
      for (size_t j = 0; j < capacities_.size(); ++j) {
        if (is_reachable[i] && !is_reachable[j] &&
            capacities_[i][j].capacity > 0) {
          ans.deleted_edges_indexes.push_back(capacities_[i][j].index);
          ans.value += capacities_[i][j].capacity;
        }
      }
    }
    return ans;
  }

 private:
  size_t start_;
  size_t end_;
  std::vector<std::vector<Edge>> capacities_;

  std::vector<bool> GetReachableMap(
      const std::vector<std::vector<int>>& flows) const {
    std::vector<bool> is_reachable(capacities_.size(), false);
    std::queue<int> queue;
    queue.push(start_);
    is_reachable[start_] = true;
    while (!queue.empty()) {
      int front = queue.front();
      queue.pop();
      for (size_t i = 0; i < capacities_[front].size(); ++i) {
        if (capacities_[front][i].capacity - flows[front][i] > 0 &&
            !is_reachable[i]) {
          is_reachable[i] = true;
          queue.push(i);
        }
      }
    }
    return is_reachable;
  }

  std::vector<std::vector<int>> GetFlowsEdmondsKarp() const {
    std::vector<std::vector<int>> flows(
        capacities_.size(), std::vector<int>(capacities_.size(), 0));
    while (true) {
      std::vector<size_t> shortest_path = GetShortestPath(flows);
      if (shortest_path.size() < 2) {
        break;
      }
      int min_capacity =
          (capacities_[shortest_path[0]][shortest_path[1]].capacity -
           flows[shortest_path[0]][shortest_path[1]]);
      for (size_t i = 1; i + 1 < shortest_path.size(); ++i) {
        min_capacity = std::min(
            min_capacity,
            capacities_[shortest_path[i]][shortest_path[i + 1]].capacity -
                flows[shortest_path[i]][shortest_path[i + 1]]);
      }
      for (size_t i = 0; i + 1 < shortest_path.size(); ++i) {
        flows[shortest_path[i]][shortest_path[i + 1]] += min_capacity;
        flows[shortest_path[i + 1]][shortest_path[i]] -= min_capacity;
      }
    }
    return flows;
  }

  std::vector<size_t> GetShortestPath(
      const std::vector<std::vector<int>>& flows) const {
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
        if (capacities_[cur][i].capacity - flows[cur][i] > 0 && !used[i]) {
          queue.push(i);
          used[i] = true;
          prev[i] = cur;
        }
      }
    }
    if (!used[end_]) {
      return {};
    }
    std::vector<size_t> ans(0);
    size_t cur = end_;
    while (cur != start_) {
      ans.push_back(cur);
      cur = prev[cur];
    }
    ans.push_back(start_);
    std::reverse(ans.begin(), ans.end());
    return ans;
  }
};

int main() {
  int n;
  int m;
  std::cin >> n >> m;
  std::vector<std::vector<Edge>> caps(n, std::vector<Edge>(n));
  std::vector<Edge> edges(m);
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int cap;
    std::cin >> start >> end >> cap;
    edges[i] = {cap, i};
    caps[start - 1][end - 1].capacity += cap;
    caps[start - 1][end - 1].index = i;
    caps[end - 1][start - 1].capacity += cap;
    caps[end - 1][start - 1].index = i;
    // caps[end - 1][start - 1] += cap;
  }
  FlowSolver solver(0, n - 1, caps);
  auto ans = solver.GetMinCut();
  std::cout << ans.deleted_edges_indexes.size() << " " << ans.value << "\n";
  for (auto x : ans.deleted_edges_indexes) {
    std::cout << x + 1 << " ";
  }
}
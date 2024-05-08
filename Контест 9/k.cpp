#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

class FlowSolver {
 public:
  FlowSolver(size_t s, size_t t, std::vector<std::vector<int>>& caps)
      : start_(s), end_(t), capacities_(caps) {}

  int GetMaxFlow() {
    std::vector<std::vector<int>> flows(
        capacities_.size(), std::vector<int>(capacities_.size(), 0));
    EdmondsKarp(flows);
    int ans = 0;
    for (size_t i = 0; i < capacities_.size(); ++i) {
      ans += flows[i][end_];
    }
    return ans;
  }

 private:
  size_t start_;
  size_t end_;
  std::vector<std::vector<int>> capacities_;

  void EdmondsKarp(std::vector<std::vector<int>>& flows) {
    while (true) {
      std::vector<size_t> shortest_path = GetShortestPath(flows);
      if (shortest_path.size() < 2) {
        break;
      }
      int min_capacity = (capacities_[shortest_path[0]][shortest_path[1]] -
                          flows[shortest_path[0]][shortest_path[1]]);
      for (size_t i = 1; i + 1 < shortest_path.size(); ++i) {
        min_capacity = std::min(
            min_capacity, capacities_[shortest_path[i]][shortest_path[i + 1]] -
                              flows[shortest_path[i]][shortest_path[i + 1]]);
      }
      for (size_t i = 0; i + 1 < shortest_path.size(); ++i) {
        flows[shortest_path[i]][shortest_path[i + 1]] += min_capacity;
        flows[shortest_path[i + 1]][shortest_path[i]] -= min_capacity;
      }
    }
  }

  std::vector<size_t> GetShortestPath(
      const std::vector<std::vector<int>>& flows) {
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
        int left_capacity = capacities_[cur][i] - flows[cur][i];
        if (left_capacity > 0 && !used[i]) {
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
  std::vector<std::vector<int>> caps(n, std::vector<int>(n, 0));
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int cap;
    std::cin >> start >> end >> cap;
    caps[start - 1][end - 1] += cap;
    // caps[end - 1][start - 1] += cap;
  }
  FlowSolver solver(0, n - 1, caps);
  std::cout << solver.GetMaxFlow();
}
  #include <algorithm>
  #include <cstddef>
  #include <iostream>
  #include <queue>
  #include <set>
  #include <vector>

  const int64_t cMaxPowerOfTwo = 60;

  class FlowSolver {
  public:
    FlowSolver(size_t s, size_t t, const std::vector<std::vector<int64_t>>& caps)
        : start_(s), end_(t), capacities_(caps) {}

    struct Flows {
      int64_t value;
      std::vector<std::vector<int64_t>> flows;
      Flows(size_t n) : value(0), flows(n, std::vector<int64_t>(n, 0)) {}
    };

    Flows GetMaxFlow() const {
      Flows ans(capacities_.size());
      ans.value = EdmondsKarp(ans.flows);
      return ans;
    }

  private:
    size_t start_;
    size_t end_;
    std::vector<std::vector<int64_t>> capacities_;

    int64_t EdmondsKarp(std::vector<std::vector<int64_t>>& flows) const {
      int64_t flow_to_end = 0;
      int64_t power = cMaxPowerOfTwo;
      int64_t resize_value = int64_t(1) << power;
      while (true) {
        if (!ShortestPathUpdateFlows(flows, resize_value)) {
          if (power == 0) {
            break;
          }
          --power;
          resize_value = int64_t(1) << power;
          continue;
        }
        flow_to_end += resize_value;
      }
      return flow_to_end;
    }

    bool ShortestPathUpdateFlows(std::vector<std::vector<int64_t>>& flows,
                                int64_t resize_value) const {
      std::queue<size_t> queue;
      std::vector<bool> used(capacities_.size(), false);
      used[start_] = true;
      queue.push(start_);
      std::vector<size_t> prev(capacities_.size(), 0);
      prev[start_] = start_;
      while (!queue.empty()) {
        auto cur = queue.front();
        queue.pop();
        for (size_t i = 0; i < capacities_[cur].size(); ++i) {
          int64_t left_capacity = capacities_[cur][i] - flows[cur][i];
          if (left_capacity >= resize_value && !used[i]) {
            queue.push(i);
            used[i] = true;
            prev[i] = cur;
          }
        }
      }
      if (!used[end_]) {
        return false;
      }

      size_t cur = end_;
      while (cur != start_) {
        flows[prev[cur]][cur] += resize_value;
        flows[cur][prev[cur]] -= resize_value;
        cur = prev[cur];
      }
      return true;
    }
  };

  int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int64_t n;
    int64_t m;
    std::cin >> n >> m;
    std::vector<std::vector<int64_t>> caps(n, std::vector<int64_t>(n, 0));
    std::vector<std::vector<std::vector<int64_t>>> map_cords_to_index(
        n, std::vector<std::vector<int64_t>>(n, std::vector<int64_t>(0)));
    std::vector<int64_t> edges_caps(m);
    std::vector<int64_t> edges_fill(m, 0);
    for (int64_t i = 0; i < m; ++i) {
      int64_t start;
      int64_t end;
      int64_t cap;
      std::cin >> start >> end >> cap;
      caps[start - 1][end - 1] += cap;
      map_cords_to_index[start - 1][end - 1].push_back(i);
      edges_caps[i] = cap;
    }
    FlowSolver solver(0, n - 1, caps);
    auto ans = solver.GetMaxFlow();
    std::cout << ans.value << "\n";
    for (size_t i = 0; i < ans.flows.size(); ++i) {
      for (size_t j = 0; j < ans.flows[i].size(); ++j) {
        int64_t left_to_use = ans.flows[i][j];
        size_t k = 0;
        while (left_to_use > 0) {
          int64_t used = std::min(edges_caps[map_cords_to_index[i][j][k]] -
                                      edges_fill[map_cords_to_index[i][j][k]],
                                  left_to_use);
          edges_fill[map_cords_to_index[i][j][k]] += used;
          left_to_use -= used;
          ++k;
        }
      }
    }
    for (auto x : edges_fill) {
      std::cout << x << "\n";
    }
  }
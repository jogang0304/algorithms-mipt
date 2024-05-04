#include <algorithm>
#include <iostream>
#include <vector>

const int cInf = 1'000'000'000;

class LCA {
 public:
  LCA(std::vector<std::vector<size_t>> graph)
      : graph_(graph),
        order_(0),
        last_index_of_vertex_in_order_(graph.size()),
        distance_from_root_(graph.size()) {
    InitializationDFS(cRoot, cRoot, 0);
    mst_ = MinSparseTable(distance_from_root_, order_);
  }

  size_t GetLCA(size_t v, size_t u) {
    size_t start = std::min(last_index_of_vertex_in_order_[v],
                            last_index_of_vertex_in_order_[u]);
    size_t end = std::max(last_index_of_vertex_in_order_[v],
                          last_index_of_vertex_in_order_[u]);
    auto ans = mst_.GetMin(start, end).index;
    return ans;
  }

  int GetDistance(size_t v, size_t u) {
    auto lca = GetLCA(v, u);
    return distance_from_root_[v] - 2 * distance_from_root_[lca] +
           distance_from_root_[u];
  }

 private:
  const size_t cRoot = 0;
  std::vector<std::vector<size_t>> graph_;
  std::vector<size_t> order_;
  std::vector<size_t> last_index_of_vertex_in_order_;
  std::vector<size_t> distance_from_root_;

  void InitializationDFS(size_t v, size_t p,
                         size_t current_distance_from_root) {
    order_.push_back(v);
    last_index_of_vertex_in_order_[v] = order_.size() - 1;
    distance_from_root_[v] = current_distance_from_root;
    for (auto next : graph_[v]) {
      if (next == p) {
        continue;
      }
      InitializationDFS(next, v, current_distance_from_root + 1);
      order_.push_back(v);
      last_index_of_vertex_in_order_[v] = order_.size() - 1;
    }
  }

  struct Statistic {
    size_t value = 0;
    size_t index = 0;
  };

  class MinSparseTable {
   public:
    MinSparseTable() = default;
    MinSparseTable(std::vector<size_t>& distances, std::vector<size_t>& order)
        : log_(order.size() + 1) {
      log_[1] = 0;
      for (size_t i = 2; i <= order.size(); i++) {
        log_[i] = log_[i / 2] + 1;
      }
      table_.resize(log_[order.size()] + 1, std::vector<Statistic>(0));
      for (size_t i = 0; i < order.size(); i++) {
        table_[0].push_back({distances[order[i]], order[i]});
      }
      for (int k = 1; k <= log_[order.size()]; k++) {
        for (int i = 0;
             i < static_cast<int>(table_[k - 1].size()) - (1 << (k - 1)); ++i) {
          if (table_[k - 1][i].value <=
              table_[k - 1][i + (1 << (k - 1))].value) {
            table_[k].push_back(table_[k - 1][i]);
          } else {
            table_[k].push_back(table_[k - 1][i + (1 << (k - 1))]);
          }
        }
      }
    }

    Statistic GetMin(int left, int right) {
      if (left > right) {
        return Statistic{cInf, 0};
      }
      int needed_range = log_[right - left + 1];
      if (table_[needed_range][left].value <=
          table_[needed_range][right - (1 << needed_range) + 1].value) {
        return table_[needed_range][left];
      }
      return table_[needed_range][right - (1 << needed_range) + 1];
    }

   private:
    std::vector<std::vector<Statistic>> table_;
    std::vector<int> log_;
  };

  MinSparseTable mst_;
};

int main() {
  size_t n;
  std::cin >> n;
  std::vector<std::vector<size_t>> graph(n, std::vector<size_t>(0));
  for (size_t i = 0; i < n - 1; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    --a;
    --b;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
  LCA lca(graph);
  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    size_t a;
    size_t b;
    std::cin >> a >> b;
    --a;
    --b;
    std::cout << lca.GetDistance(a, b) << "\n";
  }
}
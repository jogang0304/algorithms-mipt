#include <algorithm>
#include <iostream>
#include <vector>

const int cInf = 1'000'000'000;

using Graph = std::vector<std::vector<size_t>>;

class LCA {
 public:
  LCA(const Graph& graph)
      : graph_(graph),
        parent_(graph.size()),
        order_(0),
        first_index_of_vertex_in_order_(graph.size()),
        distance_from_root_(graph.size()),
        is_deleted_(graph.size(), false) {
    InitializationDFS(cRoot, cRoot, 0);
    mit_tree_ = MinSparseTable(distance_from_root_, order_);
  }

  size_t GetLCA(size_t v, size_t u) {
    size_t start = std::min(first_index_of_vertex_in_order_[v],
                            first_index_of_vertex_in_order_[u]);
    size_t end = std::max(first_index_of_vertex_in_order_[v],
                          first_index_of_vertex_in_order_[u]);
    auto ans = mit_tree_.GetMin(start, end).index;
    size_t cur = ans;
    if (is_deleted_[ans]) {
      while (is_deleted_[cur]) {
        cur = parent_[cur];
      }
      parent_[ans] = cur;
    }
    return cur;
  }

  void DeleteNode(size_t node_index) { is_deleted_[node_index] = true; }

 private:
  const size_t cRoot = 0;
  Graph graph_;
  std::vector<size_t> parent_;
  std::vector<size_t> order_;
  std::vector<size_t> first_index_of_vertex_in_order_;
  std::vector<size_t> distance_from_root_;
  std::vector<bool> is_deleted_;

  void InitializationDFS(size_t v, size_t p,
                         size_t current_distance_from_root) {
    order_.push_back(v);
    parent_[v] = p;
    first_index_of_vertex_in_order_[v] = order_.size() - 1;
    distance_from_root_[v] = current_distance_from_root;
    for (auto next : graph_[v]) {
      if (next == p) {
        continue;
      }
      InitializationDFS(next, v, current_distance_from_root + 1);
      order_.push_back(v);
    }
  }

  struct Statistic {
    size_t value = 0;  // distance to root
    size_t index = 0;  // index
    bool operator<(const Statistic& other) const { return value < other.value; }
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
          if (table_[k - 1][i] < table_[k - 1][i + (1 << (k - 1))]) {
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
      if (table_[needed_range][left] <
          table_[needed_range][right - (1 << needed_range) + 1]) {
        return table_[needed_range][left];
      }
      return table_[needed_range][right - (1 << needed_range) + 1];
    }

   private:
    std::vector<std::vector<Statistic>> table_;
    std::vector<int> log_;
  };

  MinSparseTable mit_tree_;
};

struct Question {
  char symbol;
  size_t a;
  size_t b;
};

int main() {
  size_t q;
  std::cin >> q;
  Graph graph(1);
  std::vector<Question> questions(q);
  for (size_t i = 0; i < q; ++i) {
    char c;
    std::cin >> c;
    if (c == '?') {
      size_t a;
      size_t b;
      std::cin >> a >> b;
      questions[i] = {c, a - 1, b - 1};
    } else {
      size_t v;
      std::cin >> v;
      questions[i] = {c, v - 1, 0};
      if (c == '+') {
        graph.push_back(std::vector<size_t>(0));
        graph[v - 1].push_back(graph.size() - 1);
      } 
    }
  }
  LCA lca(graph);
  for (size_t i = 0; i < questions.size(); ++i) {
    if (questions[i].symbol == '?') {
      std::cout << lca.GetLCA(questions[i].a, questions[i].b) + 1 << std::endl;
    } else if (questions[i].symbol == '-') {
      lca.DeleteNode(questions[i].a);
    }
  }
}
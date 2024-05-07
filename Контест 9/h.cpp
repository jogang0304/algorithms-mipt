#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <vector>

using SubGraph = std::vector<std::pair<size_t, size_t>>;
using Graph = std::vector<std::vector<SubGraph>>;

class MatchingBipartiteGraph {
 public:
  MatchingBipartiteGraph(Graph& g) : graph_(g) {}

  std::set<std::pair<size_t, size_t>> GetMatching() {
    std::vector<bool> used(graph_.size(), false);
    std::vector<size_t> match(graph_.size(), cMatchNullValue);
    for (size_t i = 0; i < graph_.size(); ++i) {
      Augment(used, match, i);
      used.resize(graph_.size(), false);
    }
    std::set<std::pair<size_t, size_t>> ans;
    for (size_t i = 0; i < match.size(); ++i) {
      size_t start = std::min(i, match[i]);
      size_t end = std::max(i, match[i]);
      if (end != cMatchNullValue) {
        ans.insert({start, end});
      }
    }
    return ans;
  }

 private:
  const size_t cMatchNullValue = 2'000'000'009;
  Graph graph_;

  bool Augment(std::vector<bool>& used, std::vector<size_t>& match, size_t v_x, size_t v_y) {
    if (used[v]) {
      return false;
    }
    used[v] = true;
    for (auto next : graph_[v_x][v_y]) {
      if ((match[next.first][next.second] == cMatchNullValue) ||
          Augment(used, match, match[next])) {
        match[next] = v;
        return true;
      }
    }
    return false;
  }
};

int main() {
  int n;
  int m;
  int a;
  int b;
  std::cin >> n >> m >> a >> b;
  std::vector<std::string> field(n);
  
  for (int i = 0; i < n; ++i) {
    std::string s;
    std::cin >> s;
    field[n] = s;
  }

}
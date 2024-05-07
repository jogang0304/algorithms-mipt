#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <vector>

using SubGraph = std::vector<size_t>;
using Graph = std::vector<SubGraph>;

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

  bool Augment(std::vector<bool>& used, std::vector<size_t>& match, size_t v) {
    if (used[v]) {
      return false;
    }
    used[v] = true;
    for (auto next : graph_[v]) {
      if ((match[next] == cMatchNullValue) ||
          Augment(used, match, match[next])) {
        match[next] = v;
        return true;
      }
    }
    return false;
  }
};

int main() {
  int l;
  int r;
  std::cin >> l >> r;
  Graph g(l + r, SubGraph(0));
  for (int i = 0; i < l; ++i) {
    size_t cur;
    std::cin >> cur;
    while (cur != 0) {
      g[i].push_back(cur + l - 1);
      // g[cur + l - 1].push_back(i);
      std::cin >> cur;
    }
  }
  MatchingBipartiteGraph mbg(g);
  auto res = mbg.GetMatching();
  std::cout << res.size() << "\n";
  for (auto x : res) {
    std::cout << (x.first % l) + 1 << " " << x.second - l + 1 << "\n";
  }
}
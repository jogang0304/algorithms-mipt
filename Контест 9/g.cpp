#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <vector>

using SubGraph = std::vector<int>;
using Graph = std::vector<SubGraph>;

class MatchingBipartiteGraph {
 public:
  MatchingBipartiteGraph(Graph& g) : graph_(g) {}

  std::set<std::pair<int, int>> GetMatching() {
    std::vector<bool> used(graph_.size(), false);
    std::vector<int> match(graph_.size(), cMatchNullValue);
    for (size_t i = 0; i < graph_.size(); ++i) {
      Augment(used, match, i);
      std::fill(used.begin(), used.end(), false);
    }
    std::set<std::pair<int, int>> ans;
    for (int i = 0; i < static_cast<int>(match.size()); ++i) {
      int start = std::min(i, match[i]);
      int end = std::max(i, match[i]);
      if (start != cMatchNullValue) {
        ans.insert({start, end});
      }
    }
    return ans;
  }

 private:
  const int cMatchNullValue = -1;
  Graph graph_;

  bool Augment(std::vector<bool>& used, std::vector<int>& match, int v) {
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
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
  int n;
  int m;
  int a;
  int b;
  std::cin >> n >> m >> a >> b;
  std::vector<std::string> field(n);
  int count_free = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> field[i];
    for (size_t j = 0; j < field[i].size(); ++j) {
      if (field[i][j] == '*') {
        count_free++;
      }
    }
  }
  Graph g(n * m, SubGraph(0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (field[i][j] == '.' || (i + j) % 2 == 0) {
        continue;
      }
      int index = m * i + j;
      if (index - m >= 0 && field[i - 1][j] == '*') {
        g[index].push_back(index - m);
      }
      if (index + m < n * m && field[i + 1][j] == '*') {
        g[index].push_back(index + m);
      }
      if (j + 1 < m && field[i][j + 1] == '*') {
        g[index].push_back(index + 1);
      }
      if (j - 1 >= 0 && field[i][j - 1] == '*') {
        g[index].push_back(index - 1);
      }
    }
  }
  MatchingBipartiteGraph mbg(g);
  auto count_pairs = mbg.GetMatching().size();
  int ans = count_pairs * a + (count_free - 2 * count_pairs) * b;
  std::cout << std::min(ans, count_free * b);
}
#include <cstddef>
#include <iostream>
#include <vector>

const int cSmallInf = 100'000;
const int cInf = 1'000'000'000;

std::vector<int> GetNegativeCycle(std::vector<std::vector<int>>& matrix) {
  std::vector<std::vector<int>> next_node_to_get_to(
      matrix.size(), std::vector<int>(matrix.size(), -1));
  for (size_t i = 0; i < next_node_to_get_to.size(); ++i) {
    for (size_t j = 0; j < next_node_to_get_to.size(); ++j) {
      next_node_to_get_to[i][j] = static_cast<int>(j);
    }
  }
  bool found_negative_cycle = false;
  for (size_t k = 0; (k < matrix.size()) && !found_negative_cycle; ++k) {
    for (size_t i = 0; (i < matrix.size()) && !found_negative_cycle; ++i) {
      for (size_t j = 0; (j < matrix.size()) && !found_negative_cycle; ++j) {
        if (matrix[i][j] <= matrix[i][k] + matrix[k][j]) {
          continue;
        }
        matrix[i][j] = matrix[i][k] + matrix[k][j];
        next_node_to_get_to[i][j] = next_node_to_get_to[i][k];
        if (matrix[j][j] < 0) {
          found_negative_cycle = true;
          break;
        }
      }
    }
  }
  std::vector<int> ans(0);
  for (int i = 0; i < static_cast<int>(matrix.size()); ++i) {
    if (matrix[i][i] >= 0) {
      continue;
    }
    ans.push_back(i);
    int cur = next_node_to_get_to[i][i];
    while (cur != i) {
      ans.push_back(cur);
      cur = next_node_to_get_to[cur][i];
    }
    ans.push_back(cur);
    break;
  }
  return ans;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> matrix[i][j];
      if (matrix[i][j] == cSmallInf) {
        matrix[i][j] = cInf;
      }
      if ((i == j) && matrix[i][j] > 0) {
        matrix[i][j] = 0;
      }
    }
  }
  auto res = GetNegativeCycle(matrix);
  if (res.empty()) {
    std::cout << "NO";
    return 0;
  }
  std::cout << "YES\n" << res.size() << '\n';
  for (auto x : res) {
    std::cout << x + 1 << " ";
  }
}
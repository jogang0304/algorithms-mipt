#include <iostream>
#include <vector>

void TransitiveCloseMatrix(std::vector<std::vector<bool>>& matrix) {
  for (size_t k = 0; k < matrix.size(); ++k) {
    for (size_t i = 0; i < matrix.size(); ++i) {
      for (size_t j = 0; j < matrix.size(); ++j) {
        matrix[i][j] = matrix[i][j] || (matrix[i][k] && matrix[k][j]);
      }
    }
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n, false));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int cur;
      std::cin >> cur;
      matrix[i][j] = cur == 1;
    }
  }
  TransitiveCloseMatrix(matrix);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
}
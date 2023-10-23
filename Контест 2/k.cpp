#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

int main() {
  size_t matrix_size;
  size_t submatrix_size;
  std::cin >> matrix_size >> submatrix_size;
  std::vector<std::vector<int>> matrix(matrix_size,
                                       std::vector<int>(matrix_size));
  for (auto& row : matrix) {
    for (auto& cell : row) {
      std::cin >> cell;
    }
  }
  size_t submatrixes_vector_size = matrix_size - submatrix_size + 1;
  std::vector<std::vector<int>> counted_mins(
      matrix_size, std::vector<int>(submatrixes_vector_size, kInf));
  for (size_t i = 0; i < counted_mins.size(); ++i) {
    for (size_t j = 0; j < submatrix_size; ++j) {
      counted_mins[i][0] = std::min(counted_mins[i][0], matrix[i][j]);
    }
    for (size_t j = 1; j < counted_mins[i].size(); ++j) {
      if (matrix[i][j - 1] == counted_mins[i][j - 1]) {
        for (size_t t = 0; t < submatrix_size; ++t) {
          counted_mins[i][0] = std::min(counted_mins[i][j], matrix[i][t]);
        }
      } else {
        counted_mins[i][j] =
            std::min(counted_mins[i][j - 1], matrix[i][j + submatrix_size - 1]);
      }
    }
  }

  std::vector<std::vector<int>> submatrixes(
      submatrixes_vector_size, std::vector<int>(submatrixes_vector_size, kInf));
  for (size_t i = 0; i < submatrixes_vector_size; ++i) {
    for (size_t j = 0; j < submatrixes_vector_size; ++j) {
      int cur_min = kInf;
      for (size_t t = 0; t < submatrix_size; ++t) {
        cur_min = std::min(cur_min, counted_mins[i][j + t]);
      }
      submatrixes[i][j] = cur_min;
    }
  }
  for (auto& row : submatrixes) {
    for (auto& cell : row) {
      std::cout << cell << " ";
    }
    std::cout << "\n";
  }
}
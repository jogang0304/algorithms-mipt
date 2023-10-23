#include <algorithm>
#include <iostream>
#include <vector>

const int kInf = 2'000'000'000;

struct Point {
  size_t y;
  size_t x;
  int value;
  Point(size_t y_value, size_t x_value, int value_value)
      : y(y_value), x(x_value), value(value_value) {}
};

bool PointInMatrix(Point point, size_t top, size_t left, size_t height,
                   size_t width) {
  if (point.y >= top && point.y < top + height) {
    if (point.x >= left && point.x < left + width) {
      return true;
    }
  }
  return false;
}

void CountSubmatrix(size_t submatrix_size,
                    std::vector<std::vector<Point>>& counted_mins_row,
                    size_t row, size_t column,
                    std::vector<std::vector<Point>>& submatrixes) {
  int cur_min = kInf;
  size_t cur_min_y = 0;
  size_t cur_min_x = 0;
  for (size_t shift = 0; shift < submatrix_size; ++shift) {
    if (counted_mins_row[row + shift][column].value <= cur_min) {
      cur_min = counted_mins_row[row + shift][column].value;
      cur_min_y = row + shift;
      cur_min_x = column;
    }
  }
  submatrixes[row][column].value = cur_min;
  submatrixes[row][column].y = cur_min_y;
  submatrixes[row][column].x = cur_min_x;
}

void MakeCountedMinsRow(std::vector<std::vector<Point>>& counted_mins_row,
                        size_t submatrix_size,
                        std::vector<std::vector<int>>& matrix) {
  for (size_t row = 0; row < counted_mins_row.size(); ++row) {
    size_t column = 0;
    for (size_t shift = 0; shift < submatrix_size; ++shift) {
      if (matrix[row][column + shift] <= counted_mins_row[row][column].value) {
        counted_mins_row[row][column].value = matrix[row][column + shift];
        counted_mins_row[row][column].y = row;
        counted_mins_row[row][column].x = shift;
      }
    }
    for (size_t column = 1; column < counted_mins_row[row].size(); ++column) {
      if (!PointInMatrix(counted_mins_row[row][column - 1], row, column, 1,
                         submatrix_size)) {
        for (size_t shift = 0; shift < submatrix_size; ++shift) {
          if (matrix[row][column + shift] <=
              counted_mins_row[row][column].value) {
            counted_mins_row[row][column].value = matrix[row][column + shift];
            counted_mins_row[row][column].y = row;
            counted_mins_row[row][column].x = shift;
          }
        }
      } else {
        size_t shift = submatrix_size - 1;
        if (matrix[row][column + shift] <=
            counted_mins_row[row][column - 1].value) {
          counted_mins_row[row][column].value = matrix[row][column + shift];
          counted_mins_row[row][column].y = row;
          counted_mins_row[row][column].x = shift;
        }
      }
    }
  }
}

void MakeSubmatrixes(std::vector<std::vector<Point>>& counted_mins_row,
                     size_t submatrix_size,
                     std::vector<std::vector<Point>>& submatrixes) {
  size_t submatrixes_vector_size = submatrixes.size();
  CountSubmatrix(submatrix_size, counted_mins_row, 0, 0, submatrixes);
  for (size_t row = 0; row < submatrixes_vector_size; ++row) {
    for (size_t column = 0; column < submatrixes_vector_size; ++column) {
      if (row == 0) {
        CountSubmatrix(submatrix_size, counted_mins_row, row, column,
                       submatrixes);
      } else {
        if (PointInMatrix(Point(submatrixes[row - 1][column].y,
                                submatrixes[row - 1][column].x, 0),
                          row, column, submatrix_size, submatrix_size)) {
          if (submatrixes[row - 1][column].value <
              counted_mins_row[row][column].value) {
            submatrixes[row][column] = submatrixes[row - 1][column];
          } else {
            submatrixes[row][column] = counted_mins_row[row][column];
          }
        } else {
          CountSubmatrix(submatrix_size, counted_mins_row, row, column,
                         submatrixes);
        }
      }
    }
  }
}

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
  std::vector<std::vector<Point>> counted_mins_row(
      matrix_size,
      std::vector<Point>(submatrixes_vector_size, Point(0, 0, kInf)));
  MakeCountedMinsRow(counted_mins_row, submatrix_size, matrix);

  std::vector<std::vector<Point>> submatrixes(
      submatrixes_vector_size,
      std::vector<Point>(submatrixes_vector_size, Point(0, 0, kInf)));
  MakeSubmatrixes(counted_mins_row, submatrix_size, submatrixes);
  for (auto& row : submatrixes) {
    for (auto& cell : row) {
      std::cout << cell.value << " ";
    }
    std::cout << "\n";
  }
}
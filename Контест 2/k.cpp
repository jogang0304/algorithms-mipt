#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

const int kErrorCode = -2'000'000'000;

class QueueThroughStack {
 private:
  std::stack<int> stack1_;
  std::stack<int> stack2_;
  std::stack<int> stack1_min_;
  std::stack<int> stack2_min_;
  void MoveStack1ToStack2() {
    while (!stack1_.empty()) {
      stack2_.push(stack1_.top());
      if (!stack2_min_.empty()) {
        stack2_min_.push(std::min(stack2_min_.top(), stack1_.top()));
      } else {
        stack2_min_.push(stack1_.top());
      }
      stack1_.pop();
      stack1_min_.pop();
    }
  }

 public:
  QueueThroughStack() {}
  void Push(int number) {
    stack1_.push(number);
    if (!stack1_min_.empty()) {
      stack1_min_.push(std::min(stack1_min_.top(), number));
    } else {
      stack1_min_.push(number);
    }
  }
  int Pop() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    int answer = stack2_.top();
    stack2_.pop();
    stack2_min_.pop();
    return answer;
  }
  int Front() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    return stack2_.top();
  }
  int Size() { return stack1_.size() + stack2_.size(); }
  int Min() {
    if (stack1_min_.empty() && stack2_min_.empty()) {
      return kErrorCode;
    }
    if (stack1_min_.empty()) {
      return stack2_min_.top();
    }
    if (stack2_min_.empty()) {
      return stack1_min_.top();
    }
    return std::min(stack1_min_.top(), stack2_min_.top());
  }
  void Clear() {
    stack1_ = std::stack<int>();
    stack1_min_ = std::stack<int>();
    stack2_ = std::stack<int>();
    stack2_min_ = std::stack<int>();
  }
};

int main() {
  int matrix_size;
  int submatrix_size;
  std::cin >> matrix_size >> submatrix_size;
  std::vector<std::vector<int>> matrix(matrix_size,
                                       std::vector<int>(matrix_size, 0));
  for (auto& row : matrix) {
    for (auto& element : row) {
      std::cin >> element;
    }
  }
  // минимум в столбце высоты L, начинающемся в i, j.
  std::vector<std::vector<int>> l_column_mins(matrix_size - submatrix_size + 1,
                                              std::vector<int>(matrix_size, 0));
  QueueThroughStack queue;
  for (int j = 0; j < matrix_size; ++j) {
    queue.Clear();
    for (int i = 0; i < submatrix_size - 1; ++i) {
      queue.Push(matrix[i][j]);
    }
    for (int i = 0; i < matrix_size - submatrix_size + 1; ++i) {
      if (queue.Size() >= submatrix_size) {
        queue.Pop();
      }
      queue.Push(matrix[i + submatrix_size - 1][j]);
      l_column_mins[i][j] = queue.Min();
    }
  }
  std::vector<std::vector<int>> submatrix_min(
      matrix_size - submatrix_size + 1,
      std::vector<int>(matrix_size - submatrix_size + 1, 0));
  for (int i = 0; i < matrix_size - submatrix_size + 1; ++i) {
    queue.Clear();
    for (int j = 0; j < submatrix_size - 1; ++j) {
      queue.Push(l_column_mins[i][j]);
    }
    for (int j = 0; j < matrix_size - submatrix_size + 1; ++j) {
      if (queue.Size() >= submatrix_size) {
        queue.Pop();
      }
      queue.Push(l_column_mins[i][j + submatrix_size - 1]);
      submatrix_min[i][j] = queue.Min();
    }
  }
  for (auto& row : submatrix_min) {
    for (auto& element : row) {
      std::cout << element << " ";
    }
    std::cout << "\n";
  }
}
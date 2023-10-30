#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

const int kErrorCode = -2'000'000'000;

struct El {
  int value;
  int x;
  int y;
  int z;
};

class QueueThroughStack {
 private:
  std::stack<El> stack1_;
  std::stack<El> stack2_;
  std::stack<El> stack1_max_;
  std::stack<El> stack2_max_;
  void MoveStack1ToStack2() {
    while (!stack1_.empty()) {
      stack2_.push(stack1_.top());
      if (!stack2_max_.empty()) {
        stack2_max_.push(std::max(stack2_max_.top(), stack1_.top()));
      } else {
        stack2_max_.push(stack1_.top());
      }
      stack1_.pop();
      stack1_max_.pop();
    }
  }

 public:
  QueueThroughStack() {}
  void Push(El number) {
    stack1_.push(number);
    if (!stack1_max_.empty()) {
      stack1_max_.push(std::max(stack1_max_.top(), number));
    } else {
      stack1_max_.push(number);
    }
  }
  int Pop() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    El answer = stack2_.top();
    stack2_.pop();
    stack2_max_.pop();
    return answer.value;
  }
  int Front() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    return stack2_.top().value;
  }
  int Size() { return stack1_.size() + stack2_.size(); }
  El Max() {
    if (stack1_max_.empty() && stack2_max_.empty()) {
      return {kErrorCode, 0, 0, 0};
    }
    if (stack1_max_.empty()) {
      return stack2_max_.top();
    }
    if (stack2_max_.empty()) {
      return stack1_max_.top();
    }
    return std::max(stack1_max_.top(), stack2_max_.top());
  }
  void Clear() {
    stack1_ = std::stack<El>();
    stack1_max_ = std::stack<El>();
    stack2_ = std::stack<El>();
    stack2_max_ = std::stack<El>();
  }
};

int main() {
  int X;
  int Y;
  int Z;
  int l;
  int w;
  int h;
  std::cin >> X >> Y >> Z >> l >> w >> h;
  std::vector<std::vector<std::vector<int>>> s(
      X, std::vector<std::vector<int>>(Y, std::vector<int>(Z, 0)));

  QueueThroughStack queue;

  // минимум на [x:x + l][y][z]
  std::vector<std::vector<std::vector<El>>> subcolumn_min(
      X - l + 1, std::vector<std::vector<El>>(Y, std::vector<El>(Z)));
  for (int y = 0; y < Y; ++y) {
    for (int z = 0; z < Z; ++z) {
      queue.Clear();
      for (int x = 0; x < l - 1; ++x) {
        queue.Push({s[x][y][z], x, y, z});
      }
      for (int x = 0; x < X - l + 1; ++x) {
        if (queue.Size() >= l) {
          queue.Pop();
        }
        queue.Push({s[x + l][y][z], x + l, y, z});
        subcolumn_min[x][y][z] = queue.Max();
      }
    }
  }

  // минимум на [x:x + l]][y:y + w][z]
  std::vector<std::vector<std::vector<El>>> submatrix_min(
      X - l + 1, std::vector<std::vector<El>>(Y - w + 1, std::vector<El>(Z)));
  for (int x = 0; x < X - l + 1; ++x) {
    for (int z = 0; z < Z; ++z) {
      queue.Clear();
      for (int y = 0; y < w - 1; ++y) {
        queue.Push(subcolumn_min[x][y][z]);
      }
      for (int y = 0; y < Y - w + 1; ++y) {
        if (queue.Size() >= w) {
          queue.Pop();
        }
        queue.Push(subcolumn_min[x][y + w][z]);
        submatrix_min[x][y][z] = queue.Max();
      }
    }
  }

  // минимум на [x:x + l]][y:y + w][z:z + h]
  std::vector<std::vector<std::vector<El>>> subspace_min(
      X - l + 1,
      std::vector<std::vector<El>>(Y - w + 1, std::vector<El>(Z - h + 1)));
  for (int x = 0; x < X - l + 1; ++x) {
    for (int y = 0; y < Y - w + 1; ++y) {
      queue.Clear();
      for (int z = 0; z < h - 1; ++z) {
        queue.Push(submatrix_min[x][y][z]);
      }
      for (int z = 0; z < Z - h + 1; ++z) {
        if (queue.Size() >= h) {
          queue.Pop();
        }
        queue.Push(submatrix_min[x][y][z + h]);
        subspace_min[x][y][z] = queue.Max();
      }
    }
  }

  std::vector<std::vector<std::vector<El>>> endpoints(
      X, std::vector<std::vector<El>>(Y, std::vector<El>(Z)));

  int N;
  std::cin >> N;
}

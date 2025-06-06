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

struct Dimensions {
  int x;
  int y;
  int z;
  int l;
  int w;
  int h;
};

class QueueThroughStack {
 private:
  std::stack<El> stack1_;
  std::stack<El> stack2_;
  std::stack<El> stack1_max_;
  std::stack<El> stack2_max_;
  void MoveStack1ToStack2();

 public:
  QueueThroughStack() {}
  void Push(El number);
  int Pop();
  int Size();
  El Max();
  void Clear();
};

void QueueThroughStack::MoveStack1ToStack2() {
  while (!stack1_.empty()) {
    stack2_.push(stack1_.top());
    if (!stack2_max_.empty()) {
      if (stack2_max_.top().value >= stack1_.top().value) {
        stack2_max_.push(stack2_max_.top());
      } else {
        stack2_max_.push(stack1_.top());
      }
    } else {
      stack2_max_.push(stack1_.top());
    }
    stack1_.pop();
    stack1_max_.pop();
  }
}

void QueueThroughStack::Push(El number) {
  stack1_.push(number);
  if (!stack1_max_.empty()) {
    if (stack1_max_.top().value >= number.value) {
      stack1_max_.push(stack1_max_.top());
    } else {
      stack1_max_.push(number);
    }
  } else {
    stack1_max_.push(number);
  }
}

int QueueThroughStack::Pop() {
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

int QueueThroughStack::Size() {
  return static_cast<int>(stack1_.size() + stack2_.size());
}

El QueueThroughStack::Max() {
  if (stack1_max_.empty() && stack2_max_.empty()) {
    return {kErrorCode, 0, 0, 0};
  }
  if (stack1_max_.empty()) {
    return stack2_max_.top();
  }
  if (stack2_max_.empty()) {
    return stack1_max_.top();
  }
  if (stack1_max_.top().value >= stack2_max_.top().value) {
    return stack1_max_.top();
  }
  return stack2_max_.top();
}

void QueueThroughStack::Clear() {
  stack1_ = std::stack<El>();
  stack1_max_ = std::stack<El>();
  stack2_ = std::stack<El>();
  stack2_max_ = std::stack<El>();
}

void CountSubcolumn(std::vector<std::vector<std::vector<El>>>& subcolumn_min,
                    QueueThroughStack& queue,
                    std::vector<std::vector<std::vector<int>>>& def_elems,
                    Dimensions& dims) {
  for (int y_cur = 0; y_cur < dims.y; ++y_cur) {
    for (int z_cur = 0; z_cur < dims.z; ++z_cur) {
      queue.Clear();
      for (int x_cur = 0; x_cur < dims.l - 1; ++x_cur) {
        queue.Push({def_elems[x_cur][y_cur][z_cur], x_cur, y_cur, z_cur});
      }
      for (int x_cur = 0; x_cur < dims.x; ++x_cur) {
        if (queue.Size() >= dims.l || x_cur > dims.x - dims.l) {
          queue.Pop();
        }
        if (x_cur <= dims.x - dims.l) {
          queue.Push({def_elems[x_cur + dims.l - 1][y_cur][z_cur],
                      x_cur + dims.l - 1, y_cur, z_cur});
        }
        subcolumn_min[x_cur][y_cur][z_cur] = queue.Max();
      }
    }
  }
}
void CountSubmatrix(std::vector<std::vector<std::vector<El>>>& submatrix_min,
                    std::vector<std::vector<std::vector<El>>>& subcolumn_min,
                    QueueThroughStack& queue, Dimensions& dims) {
  for (int x_cur = 0; x_cur < dims.x; ++x_cur) {
    for (int z_cur = 0; z_cur < dims.z; ++z_cur) {
      queue.Clear();
      for (int y_cur = 0; y_cur < dims.w - 1; ++y_cur) {
        queue.Push(subcolumn_min[x_cur][y_cur][z_cur]);
      }
      for (int y_cur = 0; y_cur < dims.y; ++y_cur) {
        if (queue.Size() >= dims.w || y_cur > dims.y - dims.w) {
          queue.Pop();
        }
        if (y_cur <= dims.y - dims.w) {
          queue.Push(subcolumn_min[x_cur][y_cur + dims.w - 1][z_cur]);
        }
        submatrix_min[x_cur][y_cur][z_cur] = queue.Max();
      }
    }
  }
}
void CountSubspace(std::vector<std::vector<std::vector<El>>>& subspace_min,
                   std::vector<std::vector<std::vector<El>>>& submatrix_min,
                   QueueThroughStack& queue, Dimensions& dims) {
  for (int x_cur = 0; x_cur < dims.x; ++x_cur) {
    for (int y_cur = 0; y_cur < dims.y; ++y_cur) {
      queue.Clear();
      for (int z_cur = 0; z_cur < dims.h - 1; ++z_cur) {
        queue.Push(submatrix_min[x_cur][y_cur][z_cur]);
      }
      for (int z_cur = 0; z_cur < dims.z; ++z_cur) {
        if (queue.Size() >= dims.h || z_cur > dims.z - dims.h) {
          queue.Pop();
        }
        if (z_cur <= dims.z - dims.h) {
          queue.Push(submatrix_min[x_cur][y_cur][z_cur + dims.h - 1]);
        }
        subspace_min[x_cur][y_cur][z_cur] = queue.Max();
      }
    }
  }
}

void CountEndpoints(std::vector<std::vector<std::vector<El>>>& endpoints,
                    std::vector<std::vector<std::vector<El>>>& subspace_min,
                    Dimensions& dims) {
  endpoints[dims.x - 1][dims.y - 1][dims.z - 1] = {
      subspace_min[dims.x - 1][dims.y - 1][dims.z - 1].value, dims.x - 1,
      dims.y - 1, dims.z - 1};
  for (int x_cur = dims.x - 1; x_cur >= 0; --x_cur) {
    for (int y_cur = dims.y - 1; y_cur >= 0; --y_cur) {
      for (int z_cur = dims.z - 1; z_cur >= 0; --z_cur) {
        El cur_subspace = subspace_min[x_cur][y_cur][z_cur];
        if (cur_subspace.x == x_cur && cur_subspace.y == y_cur &&
            cur_subspace.z == z_cur) {
          endpoints[x_cur][y_cur][z_cur] = cur_subspace;
        } else {
          El end_place =
              endpoints[cur_subspace.x][cur_subspace.y][cur_subspace.z];
          endpoints[x_cur][y_cur][z_cur] = {cur_subspace.value, end_place.x,
                                            end_place.y, end_place.z};
        }
      }
    }
  }
}

int main() {
  int x_val;
  int y_val;
  int z_val;
  int l_val;
  int w_val;
  int h_val;
  std::cin >> x_val >> y_val >> z_val >> l_val >> w_val >> h_val;
  Dimensions dims{x_val, y_val, z_val, l_val, w_val, h_val};
  std::vector<std::vector<std::vector<int>>> def_elems(
      x_val, std::vector<std::vector<int>>(y_val, std::vector<int>(z_val, 0)));
  for (int x_cur = 0; x_cur < x_val; ++x_cur) {
    for (int y_cur = 0; y_cur < y_val; ++y_cur) {
      for (int z_cur = 0; z_cur < z_val; ++z_cur) {
        std::cin >> def_elems[x_cur][y_cur][z_cur];
      }
    }
  }

  QueueThroughStack queue;

  // минимум на [x:x + l][y][z]
  std::vector<std::vector<std::vector<El>>> subcolumn_min(
      x_val, std::vector<std::vector<El>>(y_val, std::vector<El>(z_val)));
  CountSubcolumn(subcolumn_min, queue, def_elems, dims);

  // минимум на [x:x + l]][y:y + w][z]
  std::vector<std::vector<std::vector<El>>> submatrix_min(
      x_val, std::vector<std::vector<El>>(y_val, std::vector<El>(z_val)));
  CountSubmatrix(submatrix_min, subcolumn_min, queue, dims);

  // минимум на [x:x + l]][y:y + w][z:z + h]
  std::vector<std::vector<std::vector<El>>> subspace_min(
      x_val, std::vector<std::vector<El>>(y_val, std::vector<El>(z_val)));
  CountSubspace(subspace_min, submatrix_min, queue, dims);

  std::vector<std::vector<std::vector<El>>> endpoints(
      x_val, std::vector<std::vector<El>>(y_val, std::vector<El>(z_val)));
  CountEndpoints(endpoints, subspace_min, dims);

  int n_val;
  std::cin >> n_val;
  for (int i = 0; i < n_val; i++) {
    int start_x;
    int start_y;
    int start_z;
    std::cin >> start_x >> start_y >> start_z;
    El endpoint = endpoints[start_x][start_y][start_z];
    std::cout << endpoint.x << " " << endpoint.y << " " << endpoint.z << "\n";
  }
}
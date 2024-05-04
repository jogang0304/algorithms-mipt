#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stack>
#include <vector>

const int cRootParent = -1;

class DSU {
 public:
  DSU(size_t n) : parent_(n, cRootParent), size_(n, 1) {}
  bool InOneSet(int v, int u) { return GetRoot(v) == GetRoot(u); }
  void Unite(int v, int u) {
    if (InOneSet(v, u)) {
      return;
    }
    int v_root = GetRoot(v);
    int u_root = GetRoot(u);
    if (size_[v_root] < size_[u_root]) {
      std::swap(v_root, u_root);
    }
    parent_[u_root] = v_root;
    size_[v_root] += size_[u_root];
  }

 private:
  std::vector<int> parent_;
  std::vector<size_t> size_;
  int GetRoot(int v) {
    if (parent_[v] == cRootParent) {
      return v;
    }
    parent_[v] = GetRoot(parent_[v]);
    return parent_[v];
  }
};

struct Request {
  bool is_cut;
  int v;
  int u;
};

int main() {
  int n;
  int m;
  int k;
  std::cin >> n >> m >> k;
  for (int i = 0; i < m * 2; ++i) {
    int tmp;
    std::cin >> tmp;
  }
  std::stack<Request> requests;
  for (int i = 0; i < k; ++i) {
    std::string r_string;
    int v;
    int u;
    std::cin >> r_string >> v >> u;
    requests.push({r_string == "cut", v - 1, u - 1});
  }
  DSU dsu(n);
  std::stack<bool> answers;
  while (!requests.empty()) {
    Request top = requests.top();
    requests.pop();
    if (top.is_cut) {
      dsu.Unite(top.v, top.u);
    } else {
      answers.push(dsu.InOneSet(top.v, top.u));
    }
  }
  while (!answers.empty()) {
    std::cout << (answers.top() ? "YES\n" : "NO\n");
    answers.pop();
  }
}
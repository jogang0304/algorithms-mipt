#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

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
  const int cRootParent = -1;
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

class MST {
 public:
  using WeightT = int64_t;
  using IndexT = size_t;
  struct Edge {
    size_t start;
    size_t end;
    WeightT weight;
    IndexT index = 0;
    bool operator<(const Edge& other) const {
      if (weight != other.weight) {
        return weight < other.weight;
      }
      if (index != other.index) {
        return index < other.index;
      }
      return (start + end) < (other.start + other.end);
    }
  };

  MST(size_t n, std::set<Edge> edges) : amount_of_vertices_(n), edges_(edges) {}

  std::set<Edge> GetMinTree() const {
    std::set<Edge> ans;
    DSU dsu(amount_of_vertices_);
    for (auto e : edges_) {
      if (!dsu.InOneSet(e.start, e.end)) {
        ans.insert(e);
        dsu.Unite(e.start, e.end);
      }
    }
    return ans;
  }

  WeightT GetMinTreeWeight() const {
    WeightT ans = 0;
    for (auto e : GetMinTree()) {
      ans += e.weight;
    }
    return ans;
  }

 private:
  size_t amount_of_vertices_;
  std::set<Edge> edges_;
};

int main() {
  size_t n;
  size_t m;
  std::cin >> n >> m;
  std::set<MST::Edge> edges;
  for (size_t i = 0; i < m; ++i) {
    size_t b;
    size_t e;
    MST::WeightT w;
    std::cin >> b >> e >> w;
    b--;
    e--;
    edges.insert({b, e, w, i});
  }
  MST mst(n, edges);
  std::cout << mst.GetMinTreeWeight();
}
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int cNoneValue = -1;
const int cInf = 1'000'000'000;

struct Node {
  int index = cNoneValue;
  int time_in = cNoneValue;
  int time_out = cNoneValue;
  int min_tin_from_subtree = cNoneValue;
  int component = cNoneValue;
};

struct Edge {
  int start = cNoneValue;
  int end = cNoneValue;
  int index = cNoneValue;
};

int safest_node = 0;
std::vector<Node> nodes;
std::vector<std::vector<Edge>> graph;
std::vector<bool> edge_is_bridge;
std::vector<Node> strong_components;
std::vector<std::vector<int>> components_graph;
std::vector<std::pair<int, int>> questions;

bool operator<(const Edge& left, const Edge& right) {
  return left.start < right.start ||
         (left.start == right.start && left.end < right.end);
}

bool operator==(const Edge& left, const Edge& right) {
  return (left.start == right.start) && (left.end == right.end);
}

int timer = 0;
void DfsGetBridges(int start, int parent = cNoneValue) {
  nodes[start].time_in = timer++;
  nodes[start].min_tin_from_subtree = nodes[start].time_in;
  for (auto next : graph[start]) {
    if (next.end == parent) {
      continue;
    }
    if (nodes[next.end].time_in == cNoneValue) {
      DfsGetBridges(next.end, start);
      // for (auto element : subtree_answer) {
      //   answer.insert(element);
      // }
      if (nodes[next.end].min_tin_from_subtree > nodes[start].time_in) {
        // answer.insert({std::min(start, next), std::max(start, next)});
        edge_is_bridge[next.index] = true;
      }
    }
    nodes[start].min_tin_from_subtree =
        std::min(nodes[start].min_tin_from_subtree,
                 nodes[next.end].min_tin_from_subtree);
  }
  nodes[start].time_out = timer++;
}

void PaintToComponent(int start, int component) {
  nodes[start].component = component;
  for (auto next : graph[start]) {
    if (edge_is_bridge[next.index]) {
      continue;
    }
    if (nodes[next.end].component == cNoneValue) {
      PaintToComponent(next.end, component);
    }
  }
}

void Input() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges >> safest_node;
  --safest_node;
  graph.resize(amount_of_nodes);
  edge_is_bridge.resize(amount_of_edges, false);
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    graph[start - 1].push_back({start - 1, end - 1, i});
    graph[end - 1].push_back({end - 1, start - 1, i});
  }
  nodes.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    nodes[i].index = i;
  }

  int q;
  std::cin >> q;
  questions.resize(q);
  for (int i = 0; i < q; ++i) {
    std::cin >> questions[i].first >> questions[i].second;
    questions[i].first--;
    questions[i].second--;
  }
}

void MakeComponentsGraph(int components) {
  strong_components.resize(components);
  components_graph.resize(components);
  std::vector<bool> edge_is_added(edge_is_bridge.size(), false);
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    for (auto next : graph[i]) {
      if (nodes[i].component != nodes[next.end].component &&
          !edge_is_added[next.index]) {
        components_graph[nodes[i].component].push_back(
            nodes[next.end].component);
        components_graph[nodes[next.end].component].push_back(
            nodes[i].component);
        edge_is_added[next.index] = true;
      }
    }
  }
}

class LCA {
 public:
  LCA(const std::vector<std::vector<int>>& graph, int root)
      : root_(root),
        graph_(graph),
        order_(0),
        last_index_of_vertex_in_order_(graph.size()),
        distance_from_root_(graph.size()) {
    InitializationDFS(root_, root_, 0);
    mst_ = MinTree(distance_from_root_, order_);
  }

  int GetLCA(int v, int u) const {
    int start = std::min(last_index_of_vertex_in_order_[v],
                         last_index_of_vertex_in_order_[u]);
    int end = std::max(last_index_of_vertex_in_order_[v],
                       last_index_of_vertex_in_order_[u]);
    auto ans = mst_.GetMin(start, end + 1).index;
    return ans;
  }

  // int GetDistance(int v, int u) const {
  //   auto lca = GetLCA(v, u);
  //   return distance_from_root_[v] - 2 * distance_from_root_[lca] +
  //          distance_from_root_[u];
  // }

  int GetDistanceFromRoot(int v) const { return distance_from_root_[v]; }

 private:
  int root_;
  std::vector<std::vector<int>> graph_;
  std::vector<int> order_;
  std::vector<int> last_index_of_vertex_in_order_;
  std::vector<int> distance_from_root_;

  void InitializationDFS(int v, int p, int current_distance_from_root) {
    order_.push_back(v);
    last_index_of_vertex_in_order_[v] = order_.size() - 1;
    distance_from_root_[v] = current_distance_from_root;
    for (auto next : graph_[v]) {
      if (next == p) {
        continue;
      }
      InitializationDFS(next, v, current_distance_from_root + 1);
      order_.push_back(v);
      last_index_of_vertex_in_order_[v] = order_.size() - 1;
    }
  }

  struct Statistic {
    int value = 0;
    int index = 0;
  };

  class MinTree {
   public:
    struct Statistic {
      int value;  // distance from root
      int index;  // vertex number
      bool operator<(const Statistic& other) const {
        return value < other.value;
      }
    };

    MinTree() : size_(0), tree_(0) {}
    MinTree(std::vector<int>& distances, std::vector<int>& order)
        : size_(order.size()), tree_(order.size() * 4) {
      Build(distances, 0, 0, size_, order);
    }
    Statistic GetMin(int left, int right) const {
      return InnerGetMin(0, 0, size_, left, right);
    }

   private:
    int size_;
    std::vector<Statistic> tree_;

    void Build(const std::vector<int>& distances, int v, int tree_left,
               int tree_right, const std::vector<int>& order) {
      if (tree_left + 1 == tree_right) {
        tree_[v].value = distances[order[tree_left]];
        tree_[v].index = order[tree_left];
        return;
      }
      int m = (tree_left + tree_right) / 2;
      Build(distances, 2 * v + 1, tree_left, m, order);
      Build(distances, 2 * v + 2, m, tree_right, order);
      tree_[v] = std::min(tree_[2 * v + 1], tree_[2 * v + 2]);
    }

    Statistic InnerGetMin(int v, int tree_left, int tree_right,
                          int request_left, int request_right) const {
      if (tree_left == request_left && tree_right == request_right) {
        return tree_[v];
      }
      if (tree_left >= request_right || tree_right <= request_left) {
        return {cInf, 0};
      }
      int m = (tree_left + tree_right) / 2;
      return std::min(InnerGetMin(2 * v + 1, tree_left, m, request_left,
                                  std::min(m, request_right)),
                      InnerGetMin(2 * v + 2, m, tree_right,
                                  std::max(m, request_left), request_right));
    }
  };

  MinTree mst_;
};

int main() {
  Input();
  DfsGetBridges(0);
  int component = 0;
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    if (nodes[i].component == cNoneValue) {
      PaintToComponent(i, component++);
    }
  }
  MakeComponentsGraph(component);
  LCA lca(components_graph, nodes[safest_node].component);
  // std::cout << "SAFEST NODE COMPONENT " << nodes[safest_node].component <<
  // "\n";
  for (auto question : questions) {
    // std::cout << "QUESTION COMPONENTS " << nodes[question.first].component
    //           << " " << nodes[question.second].component << "\n";
    std::cout << lca.GetDistanceFromRoot(
                     lca.GetLCA(nodes[question.first].component,
                                nodes[question.second].component))
              << "\n";
  }
}
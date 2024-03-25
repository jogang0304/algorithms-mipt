#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int NoneValue = -1;

struct Node {
  int index = NoneValue;
  int time_in = NoneValue;
  int time_out = NoneValue;
  int min_tin_from_subtree = NoneValue;
};

struct Edge {
  int start = NoneValue;
  int end = NoneValue;
  int index = NoneValue;
};

bool operator<(const Edge& left, const Edge& right) {
  return left.start < right.start ||
         (left.start == right.start && left.end < right.end);
}

bool operator==(const Edge& left, const Edge& right) {
  return (left.start == right.start) && (left.end == right.end);
}

bool operator<(const Node& left, const Node& right) {
  return left.index < right.index;
}

bool operator==(const Node& left, const Node& right) {
  return left.index == right.index;
}

int timer = 0;
std::set<Node> DfsGetPoints(const std::vector<std::vector<int>>& graph,
                            std::vector<Node>& nodes, int start,
                            int parent = NoneValue) {
  nodes[start].time_in = timer++;
  nodes[start].min_tin_from_subtree = nodes[start].time_in;
  std::set<Node> answer;
  int different_subtrees = 0;
  for (auto next : graph[start]) {
    if (next == parent) {
      continue;
    }
    if (nodes[next].time_in == NoneValue) {
      ++different_subtrees;
      auto subtree_answer = DfsGetPoints(graph, nodes, next, start);
      for (auto element : subtree_answer) {
        answer.insert(element);
      }
      if (nodes[next].min_tin_from_subtree >= nodes[start].time_in &&
          parent != NoneValue && graph[start].size() > 1) {
        answer.insert({start, NoneValue, NoneValue, NoneValue});
      }
      nodes[start].min_tin_from_subtree = std::min(
          nodes[start].min_tin_from_subtree, nodes[next].min_tin_from_subtree);
    }
    nodes[start].min_tin_from_subtree =
        std::min(nodes[start].min_tin_from_subtree, nodes[next].time_in);
    if (parent == NoneValue && different_subtrees > 1) {
      answer.insert({start, NoneValue, NoneValue, NoneValue});
    }
  }
  nodes[start].time_out = timer++;
  return answer;
}

void PrintAnswer(std::vector<Node>& nodes, std::vector<Node>& points) {
  std::cout << points.size() << "\n";
  std::sort(nodes.begin(), nodes.end());
  std::sort(points.begin(), points.end());
  std::vector<int> bridge_indexes(0);
  for (size_t i = 0; i < points.size(); ++i) {
    if (i != 0 && points[i] == points[i - 1]) {
      continue;
    }
    auto edge = std::lower_bound(nodes.begin(), nodes.end(), points[i]);
    bridge_indexes.push_back(edge->index);
  }
  std::sort(bridge_indexes.begin(), bridge_indexes.end());
  for (auto index : bridge_indexes) {
    std::cout << index + 1 << "\n";
  }
}

int main() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges;
  std::vector<std::vector<int>> graph(amount_of_nodes, std::vector<int>(0));
  std::vector<Edge> edges(amount_of_edges);
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    graph[start - 1].push_back(end - 1);
    graph[end - 1].push_back(start - 1);
    edges[i] = {std::min(start - 1, end - 1), std::max(start - 1, end - 1), i};
  }
  std::vector<Node> nodes(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    nodes[i].index = i;
  }
  std::vector<Node> bridges(0);
  for (int i = 0; i < amount_of_nodes; ++i) {
    if (nodes[i].time_in == NoneValue) {
      auto answer = DfsGetPoints(graph, nodes, i);
      for (auto element : answer) {
        bridges.push_back(element);
      }
    }
  }
  PrintAnswer(nodes, bridges);
}
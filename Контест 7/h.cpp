#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

const int NoneValue = -1;
const int MagicValue = 3;

struct Node {
  int index = NoneValue;
  int time_in = NoneValue;
  int time_out = NoneValue;
  int min_tin_from_subtree = NoneValue;
  int component = NoneValue;
};

struct Edge {
  int start = NoneValue;
  int end = NoneValue;
  int index = NoneValue;
};

std::vector<Node> nodes;
std::vector<std::vector<int>> graph;
std::vector<Node> strong_components;
std::vector<Edge> components_graph;

bool operator<(const Edge& left, const Edge& right) {
  return left.start < right.start ||
         (left.start == right.start && left.end < right.end);
}

bool operator==(const Edge& left, const Edge& right) {
  return (left.start == right.start) && (left.end == right.end);
}

int timer = 0;
std::vector<Edge> DfsGetBridges(int start, int component,
                                int parent = NoneValue) {
  nodes[start].time_in = timer++;
  nodes[start].component = component;
  nodes[start].min_tin_from_subtree = nodes[start].time_in;
  std::vector<Edge> answer;
  bool visited_parent = false;
  for (auto next : graph[start]) {
    if (next == parent && !visited_parent) {
      visited_parent = true;
      continue;
    }
    if (nodes[next].time_in == NoneValue) {
      auto subtree_answer = DfsGetBridges(next, component, start);
      for (auto element : subtree_answer) {
        answer.push_back(element);
      }
      if (nodes[next].min_tin_from_subtree > nodes[start].time_in) {
        answer.push_back(
            {std::min(start, next), std::max(start, next), NoneValue});
      }
    }
    nodes[start].min_tin_from_subtree = std::min(
        nodes[start].min_tin_from_subtree, nodes[next].min_tin_from_subtree);
  }
  nodes[start].time_out = timer++;
  return answer;
}

struct BridgesInfo {
  int amount_of_bridges;
  int amount_of_comopnents;
  std::vector<std::vector<Edge>> bridges_by_components;
};

void Input() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges;
  graph.resize(amount_of_nodes, std::vector<int>(0));
  std::vector<Edge> edges(amount_of_edges);
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    graph[start - 1].push_back(end - 1);
    graph[end - 1].push_back(start - 1);
    edges[i] = {std::min(start - 1, end - 1), std::max(start - 1, end - 1), i};
  }
  nodes.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    nodes[i].index = i;
  }
}

BridgesInfo GetBridgesInfo() {
  int component = 0;
  int amount_of_bridges = 0;
  std::vector<std::vector<Edge>> bridges_by_component(0);
  for (size_t i = 0; i < nodes.size(); ++i) {
    if (nodes[i].time_in == NoneValue) {
      auto answer = DfsGetBridges(static_cast<int>(i), component);
      bridges_by_component.push_back(answer);
      amount_of_bridges += static_cast<int>(answer.size());
      ++component;
    }
  }
  return {amount_of_bridges, component, bridges_by_component};
}

void DfsPaint(int start, int color) {}

void ParseNodesToComponents(std::vector<Edge> bridges) {
  std::set<Edge> bridges_set;
  for (size_t i = 0; i < bridges.size(); ++i) {
    bridges_set.insert(bridges[i]);
  }
}

int main() {
  Input();
  auto bridges_info = GetBridgesInfo();
  strong_components.resize(bridges_info.amount_of_bridges + 1);
  ParseNodesToComponents(bridges_info.bridges_by_components[0]);
  std::cout << (bridges_info.amount_of_bridges + 1) / MagicValue + 1;
}
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
  int component = NoneValue;
};

struct Edge {
  int start = NoneValue;
  int end = NoneValue;
};

std::vector<Node> nodes;
std::vector<std::set<int>> graph;
std::vector<Node> strong_components;
std::vector<std::set<int>> components_graph;

bool operator<(const Edge& left, const Edge& right) {
  return left.start < right.start ||
         (left.start == right.start && left.end < right.end);
}

bool operator==(const Edge& left, const Edge& right) {
  return (left.start == right.start) && (left.end == right.end);
}

int timer = 0;
std::set<Edge> DfsGetBridges(int start, int parent = NoneValue) {
  nodes[start].time_in = timer++;
  nodes[start].min_tin_from_subtree = nodes[start].time_in;
  std::set<Edge> answer;
  for (auto next : graph[start]) {
    if (next == parent) {
      continue;
    }
    if (nodes[next].time_in == NoneValue) {
      auto subtree_answer = DfsGetBridges(next, start);
      for (auto element : subtree_answer) {
        answer.insert(element);
      }
      if (nodes[next].min_tin_from_subtree > nodes[start].time_in) {
        answer.insert({std::min(start, next), std::max(start, next)});
      }
    }
    nodes[start].min_tin_from_subtree = std::min(
        nodes[start].min_tin_from_subtree, nodes[next].min_tin_from_subtree);
  }
  nodes[start].time_out = timer++;
  return answer;
}

void PaintToComponent(int start, int component,
                      const std::set<Edge>& deleted_edges) {
  nodes[start].component = component;
  for (auto next : graph[start]) {
    if (deleted_edges.find({start, next}) != deleted_edges.end() ||
        deleted_edges.find({next, start}) != deleted_edges.end()) {
      continue;
    }
    if (nodes[next].component == NoneValue) {
      PaintToComponent(next, component, deleted_edges);
    }
  }
}

void Input() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges;
  graph.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    graph[start - 1].insert(end - 1);
    graph[end - 1].insert(start - 1);
  }
  nodes.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    nodes[i].index = i;
  }
}

void MakeComponentsGraph(int components) {
  strong_components.resize(components);
  components_graph.resize(components);
  for (size_t i = 0; i < nodes.size(); ++i) {
    for (auto next : graph[i]) {
      if (nodes[i].component != nodes[next].component) {
        components_graph[nodes[i].component].insert(nodes[next].component);
        components_graph[nodes[next].component].insert(nodes[i].component);
      }
    }
  }
}

std::vector<int> GetComponentsLeafs() {
  std::vector<int> answer;
  for (size_t i = 0; i < strong_components.size(); ++i) {
    if (components_graph[i].size() == 1) {
      answer.push_back(i);
    }
  }
  return answer;
}

int main() {
  Input();
  auto bridges = DfsGetBridges(0);
  int component = 0;
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    if (nodes[i].component == NoneValue) {
      PaintToComponent(i, component++, bridges);
    }
  }
  MakeComponentsGraph(component);
  auto leafs = GetComponentsLeafs();
  std::cout << (leafs.size() + 1) / 2;
}
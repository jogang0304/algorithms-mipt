#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

const int NoneValue = -1;

std::vector<std::set<int>> normal_graph;
std::vector<std::set<int>> reversed_graph;
std::vector<std::set<int>> component_graph;

struct Node {
  int index = NoneValue;
  int time_in = NoneValue;
  int time_out = NoneValue;
  int component = NoneValue;
};

std::vector<Node> normal_nodes;
std::vector<Node> reverse_nodes;
std::vector<Node> component_nodes;

void MakeNormalGraph() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges;
  normal_graph.resize(amount_of_nodes);
  normal_nodes.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    normal_nodes[i].index = i;
  }
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    normal_graph[start - 1].insert(end - 1);
  }
}

void MakeReversedGraph() {
  reversed_graph.resize(normal_graph.size());
  reverse_nodes.resize(normal_nodes.size());
  for (size_t i = 0; i < reversed_graph.size(); ++i) {
    reverse_nodes[i].index = static_cast<int>(i);
    for (auto node : normal_graph[i]) {
      reversed_graph[node].insert(i);
    }
  }
}

void MakeComponentGraph() {
  int amount_of_components = 0;
  for (size_t i = 0; i < reverse_nodes.size(); ++i) {
    amount_of_components =
        std::max(amount_of_components, reverse_nodes[i].component + 1);
  }
  component_graph.resize(amount_of_components);
  component_nodes.resize(amount_of_components);
  for (size_t i = 0; i < reverse_nodes.size(); ++i) {
    component_nodes[reverse_nodes[i].component].index =
        reverse_nodes[i].component;
    for (auto node : normal_graph[reverse_nodes[i].index]) {
      if (reverse_nodes[node].component != reverse_nodes[i].component) {
        component_graph[reverse_nodes[i].component].insert(
            reverse_nodes[node].component);
      }
    }
  }
}

int timer = 0;
void Dfs(std::vector<std::set<int>>& graph, std::vector<Node>& nodes,
         size_t start, int component = 0) {
  nodes[start].time_in = timer++;
  nodes[start].component = component;
  for (auto next : graph[start]) {
    if (next == static_cast<int>(start)) {
      continue;
    }
    if (nodes[next].time_in == NoneValue) {
      Dfs(graph, nodes, next, component);
    }
  }
  nodes[start].time_out = timer++;
}

void SortNormalNodesByTimeOut() {
  for (size_t i = 0; i < normal_nodes.size(); ++i) {
    if (normal_nodes[i].time_in == NoneValue) {
      Dfs(normal_graph, normal_nodes, i);
    }
  }
  std::sort(normal_nodes.begin(), normal_nodes.end(),
            [](const Node& left, const Node& right) {
              return left.time_out > right.time_out;
            });
}

void SplitReverseNodesToComponents() {
  int component = 0;
  for (size_t i = 0; i < normal_nodes.size(); ++i) {
    if (reverse_nodes[normal_nodes[i].index].time_in == NoneValue) {
      Dfs(reversed_graph, reverse_nodes, normal_nodes[i].index, component++);
    }
  }
}

void TopologicallySortComponents() {
  for (size_t i = 0; i < component_nodes.size(); ++i) {
    if (component_nodes[i].time_in == NoneValue) {
      Dfs(component_graph, component_nodes, i);
    }
  }
  std::sort(component_nodes.begin(), component_nodes.end(),
            [](const Node& left, const Node& right) {
              return left.time_out > right.time_out;
            });
}

void PrintAnswer() {
  std::vector<int> map_index_to_component(normal_nodes.size());
  for (size_t i = 0; i < normal_nodes.size(); ++i) {
    map_index_to_component[i] = reverse_nodes[i].component;
  }
  std::vector<int> map_component_to_sorted(component_nodes.size());
  for (size_t i = 0; i < component_nodes.size(); ++i) {
    map_component_to_sorted[component_nodes[i].index] = i;
  }
  std::cout << component_nodes.size() << "\n";
  for (size_t i = 0; i < normal_nodes.size(); ++i) {
    std::cout << map_component_to_sorted[map_index_to_component[i]] + 1 << " ";
  }
}

int main() {
  MakeNormalGraph();
  MakeReversedGraph();
  SortNormalNodesByTimeOut();
  SplitReverseNodesToComponents();
  MakeComponentGraph();
  TopologicallySortComponents();
  PrintAnswer();
}
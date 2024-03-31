#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

const int NoneValue = -1;

struct Edge {
  int start = NoneValue;
  int end = NoneValue;
  bool is_black = false;
};

struct Node {
  std::set<int> edges;
  bool is_black = false;
  int amount_of_black_edges = 0;
  int time_in = NoneValue;
  int time_out = NoneValue;
};

std::vector<Edge> edges;
std::vector<Node> nodes;

int timer = 0;
void Dfs(int start) {
  nodes[start].time_in = timer++;
  for (auto edge : nodes[start].edges) {
    auto next = start == edges[edge].start ? edges[edge].end : edges[edge].start;
    if (nodes[start].is_black && nodes[start].amount_of_black_edges % 2 == 0) {
      edges[edge].is_black = true;
      nodes[next].amount_of_black_edges++;
    }
  }
  nodes[start].time_out = timer++;

}


int main() {
  int amount_of_tests;
  std::cin >> amount_of_tests;
  for (int test = 0; test < amount_of_tests; ++test) {
    int amount_of_nodes;
    int amount_of_edges;
    std::cin >> amount_of_nodes >> amount_of_edges;
    edges.resize(amount_of_edges);
    nodes.resize(amount_of_nodes);
    for (int i = 0; i < amount_of_edges; ++i) {
      int start;
      int end;
      std::cin >> start >> end;
      edges[i].start = start - 1;
      edges[i].end = end - 1;
      nodes[start - 1].edges.insert(i);
      nodes[end - 1].edges.insert(i);
    }
    std::string color_bits;
    std::cin >> color_bits;
    for (int i = 0; i < amount_of_edges; ++i) {
      edges[i].is_black = color_bits[i] == '1';
    }
  }
}
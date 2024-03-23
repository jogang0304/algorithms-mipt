#include <algorithm>
#include <iostream>
#include <vector>

const int NoneValue = -1;

struct Node {
  int index = NoneValue;
  int time_in = NoneValue;
  int time_out = NoneValue;
};

std::vector<Node> nodes;
std::vector<std::vector<int>> graph;

int timer = 0;
bool DfsHasCycle(int start_node) {
  nodes[start_node].time_in = timer++;
  bool answer = false;
  for (int next : graph[start_node]) {
    if (nodes[next].time_in != NoneValue && nodes[next].time_out == NoneValue) {
      return true;
    }
    if (nodes[next].time_in == NoneValue) {
      answer = DfsHasCycle(next);
      if (answer) {
        return true;
      }
    }
  }
  nodes[start_node].time_out = timer++;
  return answer;
}

int main() {
  int amount_of_nodes;
  int amount_of_edges;
  std::cin >> amount_of_nodes >> amount_of_edges;
  nodes.resize(amount_of_nodes);
  for (int i = 0; i < amount_of_nodes; ++i) {
    nodes[i].index = i;
  }
  graph.resize(amount_of_nodes, std::vector<int>(0));
  for (int i = 0; i < amount_of_edges; ++i) {
    int start;
    int end;
    std::cin >> start >> end;
    graph[start - 1].push_back(end - 1);
  }
  bool possible = true;
  for (int i = 0; i < amount_of_nodes; ++i) {
    if (nodes[i].time_in == NoneValue) {
      bool answer = DfsHasCycle(i);
      if (answer) {
        possible = false;
        break;
      }
    }
  }
  if (!possible) {
    std::cout << -1;
    return 0;
  }
  std::sort(nodes.begin(), nodes.end(),
            [](const Node& left, const Node& right) {
              return left.time_out > right.time_out;
            });
  for (int i = 0; i < amount_of_nodes; ++i) {
    std::cout << nodes[i].index + 1 << " ";
  }
}
#include <iostream>
#include <set>
#include <vector>

const int cInf = 2009000999;

struct Path {
  int16_t destination;
  int64_t length;
};

auto FindRoutesLengthsFromStartNode(const std::vector<std::vector<Path>>& graph,
                                    int16_t start_node) {
  std::vector<int64_t> ans(graph.size(), cInf);
  ans[start_node] = 0;
  std::set<std::pair<int64_t, int16_t>> set;
  set.insert({ans[start_node], start_node});
  while (!set.empty()) {
    auto begin_node = *set.begin();
    set.erase(set.begin());
    for (const auto& path : graph[begin_node.second]) {
      auto new_length = ans[begin_node.second] + path.length;
      if (ans[path.destination] != cInf &&
          ans[path.destination] <= new_length) {
        continue;
      }
      set.erase({ans[path.destination], path.destination});
      ans[path.destination] = new_length;
      set.insert({ans[path.destination], path.destination});
    }
  }
  return ans;
}

auto GetGraph() {
  int amount_of_rooms;
  int amount_of_paths;
  std::cin >> amount_of_rooms >> amount_of_paths;
  std::vector<std::vector<Path>> graph(amount_of_rooms, std::vector<Path>(0));
  for (int i = 0; i < amount_of_paths; ++i) {
    int16_t start;
    int16_t end;
    int length;
    std::cin >> start >> end >> length;
    graph[start].push_back(Path(end, length));
    graph[end].push_back(Path(start, length));
  }
  return graph;
}

void PrintResult(const std::vector<int64_t>& result) {
  for (const auto& x : result) {
    std::cout << x << " ";
  }
  std::cout << "\n";
}

void ProcessOneBlock() {
  auto graph = GetGraph();
  int16_t start_room;
  std::cin >> start_room;
  auto result = FindRoutesLengthsFromStartNode(graph, start_room);
  PrintResult(result);
}

int main() {
  int16_t amount_of_blocks;
  std::cin >> amount_of_blocks;
  for (int16_t i = 0; i < amount_of_blocks; ++i) {
    ProcessOneBlock();
  }
}